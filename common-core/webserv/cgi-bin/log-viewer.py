#!/usr/bin/env python3
import os
import sys
import json
import re
from datetime import datetime
from collections import deque

def parse_log_line(line):
    """Parse a log line and extract structured data"""
    # Remove ANSI color codes
    line = re.sub(r'\033\[[0-9;]*m', '', line.strip())
    
    timestamp = "Unknown"
    level = "UNKNOWN"
    message = line
    
    # Look for timestamp pattern [HH:MM:SS]
    timestamp_match = re.search(r'\[(\d{2}:\d{2}:\d{2})\]', line)
    if timestamp_match:
        timestamp = timestamp_match.group(1)
        # Remove timestamp from message
        message = re.sub(r'\[\d{2}:\d{2}:\d{2}\]\s*', '', message)
    
    level_match = re.search(r'\[(DEBUG|INFO|WARNING|ERROR|CRITICAL)\]', message)
    if level_match:
        level = level_match.group(1)
        # Remove level from message
        message = re.sub(r'\[(DEBUG|INFO|WARNING|ERROR|CRITICAL)\]\s*', '', message)
    else:
        level = "UNKNOWN"
    
    return {
        "timestamp": timestamp,
        "level": level,
        "message": message.strip(),
        "raw_line": line
    }

def read_logs(max_lines=100, level_filter='ALL'):
    possible_paths = [
        os.environ.get('LOG_FILE', ''),
        'webserv.log',
        '../webserv.log',
        '/tmp/webserv.log'
    ]
    
    log_file = None
    for path in possible_paths:
        if path and os.path.exists(path):
            log_file = path
            break
    
    if not log_file:
        return {
            "error": "Log file not found",
            "searched_paths": possible_paths
        }
    
    try:
        file_size = os.path.getsize(log_file)
        
        if file_size > 10 * 1024 * 1024:  # 10MB
            return read_logs_tail(log_file, max_lines, level_filter)
        
        return read_logs_in_memory(log_file, max_lines, level_filter)
        
    except Exception as e:
        return {"error": f"Error reading log file: {str(e)}"}

def read_logs_in_memory(log_file, max_lines, level_filter='ALL'):
    """Read logs efficiently for smaller files with level filtering"""
    parsed_logs = deque(maxlen=max_lines)
    raw_lines = deque(maxlen=max_lines)
    
    with open(log_file, 'r') as f:
        for line in f:
            if line.strip():
                raw_lines.append(line)
                parsed_log = parse_log_line(line)
                if parsed_log:
                    if level_filter == 'ALL' or parsed_log['level'] == level_filter:
                        parsed_logs.append(parsed_log)
    
    return {
        "logs": list(parsed_logs),
        "total_lines": len(parsed_logs),
        "log_file": log_file,
        "file_size": f"{os.path.getsize(log_file) / 1024:.1f} KB",
        "level_filter": level_filter
    }

def read_logs_tail(log_file, max_lines, level_filter='ALL'):
    """Use system tail command for accurate line counting"""
    try:
        import subprocess
        
        # Use system tail - it's designed exactly for this purpose
        result = subprocess.run(
            ['tail', '-n', str(max_lines), log_file],
            capture_output=True, 
            text=True, 
            timeout=10
        )
        
        if result.returncode == 0:
            parsed_logs = []
            for line in result.stdout.splitlines():
                if line.strip():
                    parsed_log = parse_log_line(line)
                    if parsed_log:
                        # Apply level filter
                        if level_filter == 'ALL' or parsed_log['level'] == level_filter:
                            parsed_logs.append(parsed_log)
            
            return {
                "logs": parsed_logs,
                "total_lines": len(parsed_logs),
                "log_file": log_file,
                "file_size": f"{os.path.getsize(log_file) / 1024:.1f} KB",
                "method": "system_tail"
            }
    except Exception as e:
        pass
    
    # Fallback: read entire file but limit output
    parsed_logs = deque(maxlen=max_lines)
    total_lines = 0
    
    with open(log_file, 'r') as f:
        for line in f:
            total_lines += 1
            if line.strip():
                parsed_log = parse_log_line(line)
                if parsed_log:
                    # Apply level filter
                    if level_filter == 'ALL' or parsed_log['level'] == level_filter:
                        parsed_logs.append(parsed_log)
    
    return {
        "logs": list(parsed_logs),
        "total_lines": len(parsed_logs),
        "log_file": log_file,
        "file_size": f"{os.path.getsize(log_file) / 1024:.1f} KB"
    }

def read_logs_ultra_fast(log_file, max_lines):
    """Ultra-fast version using system 'tail' command when available"""
    try:
        import subprocess
        # Use system tail command for maximum speed
        result = subprocess.run(['tail', '-n', str(max_lines), log_file], 
                              capture_output=True, text=True, timeout=5)
        
        if result.returncode == 0:
            parsed_logs = []
            for line in result.stdout.splitlines():
                if line.strip():
                    parsed_log = parse_log_line(line)
                    if parsed_log:
                        parsed_logs.append(parsed_log)
            
            return {
                "logs": parsed_logs,
                "total_lines": len(parsed_logs),
                "log_file": log_file,
                "file_size": f"{os.path.getsize(log_file) / 1024:.1f} KB",
                "method": "system_tail"
            }
    except (ImportError, subprocess.TimeoutExpired, FileNotFoundError):
        pass
    
    # Fallback to Python implementation
    return read_logs_in_memory(log_file, max_lines)

def main():
    query_string = os.environ.get('QUERY_STRING', '')
    params = {}
    if query_string:
        for param in query_string.split('&'):
            if '=' in param:
                key, value = param.split('=', 1)
                params[key] = value
    
    max_lines = int(params.get('lines', 100))
    level_filter = params.get('level', 'ALL')  # Add level filter parameter
    
    result = read_logs(max_lines, level_filter)  # Pass level filter
    
    print("Content-Type: application/json")
    print("Access-Control-Allow-Origin: *")
    print()
    
    print(json.dumps(result, indent=2))

if __name__ == "__main__":
    main()
