// Log viewer state
let logLines = 50;
let logLevel = 'ALL';
let autoRefreshInterval = null;
let REFRESH_INTERVAL = 1000; // 1 second
let isRequestInProgress = false; // Prevent multiple simultaneous requests

document.addEventListener('DOMContentLoaded', () => {
    init();
    
    window.addEventListener('beforeunload', () => {
        if (autoRefreshInterval) {
            clearInterval(autoRefreshInterval);
        }
    });
});

function init() {
    bindEvents();
    loadLogs();
    startAutoRefresh();
}

function bindEvents() {
    document.getElementById('logLinesSelect').addEventListener('change', () => {
        applyConfiguration();
    });
    
    // Add level filter event listener
    document.getElementById('logLevelSelect').addEventListener('change', () => {
        applyConfiguration();
    });
}

function applyConfiguration() {
    const linesSelect = document.getElementById('logLinesSelect');
    const levelSelect = document.getElementById('logLevelSelect');
    const newLogLines = parseInt(linesSelect.value);
    const newLogLevel = levelSelect.value;
    
    let configChanged = false;
    
    if (newLogLines !== logLines) {
        logLines = newLogLines;
        configChanged = true;
    }
    
    if (newLogLevel !== logLevel) {
        logLevel = newLogLevel;
        configChanged = true;
    }
    
    if (configChanged) {
        loadLogs();
    }
}

async function loadLogs() {
    if (isRequestInProgress) {
        console.log('Request already in progress, skipping...');
        return;
    }
    
    isRequestInProgress = true;
    
    try {
        updateStatus('Loading logs...');
        
        const params = new URLSearchParams();
        params.append('lines', logLines);
        if (logLevel !== 'ALL') {
            params.append('level', logLevel);
        }
        
        const queryString = params.toString();
        const fullUrl = `/log-viewer.py${queryString ? '?' + queryString : ''}`;
        
        // Add timeout to prevent hanging requests
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 10000); // 10 second timeout
        
        const response = await fetch(fullUrl, {
            signal: controller.signal
        });
        
        clearTimeout(timeoutId);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}: ${response.statusText}`);
        }

        const data = await response.json();
        
        if (data.error) {
            displayError(data.error);
            updateStatus('Error loading logs');
            return;
        }
        
        displayLogs(data.logs);
        updateLastUpdated();
        
    } catch (error) {
        console.error('Error loading logs:', error);
        
        if (error.name === 'AbortError') {
            displayError('Request timed out - server may be slow');
            updateStatus('Request timed out');
        } else {
            displayError(`Failed to load logs: ${error.message}`);
            updateStatus('Error loading logs');
        }
    } finally {
        isRequestInProgress = false;
    }
}

function displayLogs(logs) {
    const container = document.getElementById('logContainer');
    
    if (!logs || logs.length === 0) {
        container.innerHTML = `
            <div class="text-center text-gray-500 py-8">
                <div class="text-2xl mb-2">📭</div>
                <div>No logs found${logLevel !== 'ALL' ? ` for level: ${logLevel}` : ''}</div>
            </div>
        `;
        return;
    }

    const logHtml = logs.map(log => formatLogEntry(log)).join('');
    container.innerHTML = logHtml;
    
    // Scroll to bottom to show latest logs
    container.scrollTop = container.scrollHeight;
}

function formatLogEntry(log) {
    const levelClass = getLevelClass(log.level);
    const timestamp = log.timestamp || 'Unknown';
    const message = log.message || log.raw_line || 'No message';
    
    return `
        <div class="log-entry ${levelClass} mb-2 p-1 rounded border-l-4 ${getLevelBorder(log.level)}">
            <div class="flex items-start space-x-3">
                <span class="text-xs text-gray-400">${timestamp}</span>
                <span class="log-level px-3 text-xs font-bold rounded min-w-[70px] text-center ${getLevelBadge(log.level)}">
                    ${log.level || 'UNKNOWN'}
                </span>
                <span class="log-message flex-1 text-xs">${escapeHtml(message)}</span>
                </div>
        </div>
    `;
}

function getLevelClass(level) {
    const levelMap = {
        'DEBUG': 'text-cyan-300',
        'INFO': 'text-green-300',
        'WARNING': 'text-yellow-300',
        'ERROR': 'text-red-300',
        'CRITICAL': 'text-red-500 font-bold'
    };
    return levelMap[level] || 'text-white';
}

function getLevelBorder(level) {
    const borderMap = {
        'DEBUG': 'border-l-cyan-400',
        'INFO': 'border-l-green-400',
        'WARNING': 'border-l-yellow-400',
        'ERROR': 'border-l-red-400',
        'CRITICAL': 'border-l-red-500'
    };
    return borderMap[level] || 'border-l-gray-400';
}

function getLevelBadge(level) {
    const badgeMap = {
        'DEBUG': 'bg-cyan-400/20 text-cyan-300 border border-cyan-400/30',
        'INFO': 'bg-green-400/20 text-green-300 border border-green-400/30',
        'WARNING': 'bg-yellow-400/20 text-yellow-300 border border-yellow-400/30',
        'ERROR': 'bg-red-400/20 text-red-300 border border-red-400/30',
        'CRITICAL': 'bg-red-500/20 text-red-400 border border-red-500/30'
    };
    return badgeMap[level] || 'bg-gray-400/20 text-gray-300 border border-gray-400/30';
}

function displayError(message) {
    const container = document.getElementById('logContainer');
    container.innerHTML = `
        <div class="text-center text-red-400 py-8">
            <div class="text-2xl mb-2">❌</div>
            <div>${escapeHtml(message)}</div>
        </div>
    `;
}

function updateStatus(message) {
    document.getElementById('logStatus').textContent = message;
}

function updateLastUpdated() {
    const now = new Date();
    const timeString = now.toLocaleTimeString();
    document.getElementById('lastUpdated').textContent = `Last updated: ${timeString}`;
}

function escapeHtml(text) {
    if (typeof text !== 'string') {
        return String(text);
    }
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

function startAutoRefresh() {
    if (autoRefreshInterval) {
        clearInterval(autoRefreshInterval);
    }
    
    autoRefreshInterval = setInterval(() => {
        if (!isRequestInProgress) {
            loadLogs();
        }
    }, REFRESH_INTERVAL);
}
