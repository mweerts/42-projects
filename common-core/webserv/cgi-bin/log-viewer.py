#!/usr/bin/env python3
import os
import sys
import json
import re
from datetime import datetime

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
    
    # Look for log level [LEVEL]
    level_match = re.search(r'\[(INFO|WARNING|ERROR|CRITICAL)\]', message)
    if level_match:
        level = level_match.group(1)
        # Remove level from message
        message = re.sub(r'\[(INFO|WARNING|ERROR|CRITICAL)\]\s*', '', message)
    else:
        return None
    
    return {
        "timestamp": timestamp,
        "level": level,
        "message": message.strip(),
        "raw_line": line
    }

def read_logs(max_lines=100):
    """Read logs from the webserv log file"""
    possible_paths = [
        os.environ.get('LOG_FILE', ''),  # From environment variable
        'webserv.log',                   # Current directory
        '../webserv.log',                # Parent directory
        '/tmp/webserv.log'               # Common temp location
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
        with open(log_file, 'r') as f:
            lines = f.readlines()
        
        parsed_logs = []
        for line in lines:
            if line.strip():  # Skip empty lines
                parsed_log = parse_log_line(line)
                if parsed_log:
                    parsed_logs.append(parsed_log)
        
        if len(parsed_logs) > max_lines:
            parsed_logs = parsed_logs[-max_lines:]
        
        return {
            "logs": parsed_logs,
            "total_lines": len(parsed_logs),
            "log_file": log_file,
            "file_size": f"{os.path.getsize(log_file) / 1024:.1f} KB"
        }
        
    except Exception as e:
        return {
            "error": f"Error reading log file: {str(e)}",
            "log_file": log_file
        }

def main():
    # Get query parameters
    query_string = os.environ.get('QUERY_STRING', '')
    params = {}
    if query_string:
        for param in query_string.split('&'):
            if '=' in param:
                key, value = param.split('=', 1)
                params[key] = value
    
    max_lines = int(params.get('lines', 100))
    
    # Read and parse logs
    result = read_logs(max_lines)
    
    # Output JSON
    print("Content-Type: application/json")
    print("Access-Control-Allow-Origin: *")  # Allow CORS if needed
    print()
    
    # Convert to JSON and print
    print(json.dumps(result, indent=2))

if __name__ == "__main__":
    main()
