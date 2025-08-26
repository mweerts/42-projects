// Configuration
const STATUS_ENDPOINT = '/status';
const REFRESH_INTERVAL = 250	; // 500ms = 2 updates per second
let lastUpdateTime = new Date();
let isRefreshing = false;

const getStatusColor = (usage) => {
    if (usage < 50) return 'text-status-green';
    if (usage < 80) return 'text-status-yellow';
    return 'text-status-red';
};

const getStatusText = (usage) => {
    if (usage < 50) return 'Healthy';
    if (usage < 80) return 'Warning';
    return 'Critical';
};

function updateRefreshInterval() {
    document.getElementById('refresh-interval').textContent = `Every ${REFRESH_INTERVAL}ms`;
}

function updateProgressBar(id, percentage, colorClass = '') {
    const progressBar = document.getElementById(id);
    if (progressBar) {
        progressBar.style.width = `${percentage}%`;
        if (colorClass) {
            progressBar.className = `h-2 rounded-full transition-all duration-300 ${colorClass}`;
        }
    }
}

function updateServerMetrics(data) {
    const connectionPercentage = Math.min((data.connections?.active || 0) / 1000 * 100, 100);
    updateProgressBar('connections-progress', connectionPercentage, 'bg-blue-600');
    document.getElementById('connections-usage').textContent = `${Math.round(connectionPercentage)}%`;
    document.getElementById('connections-active').textContent = `${data.connections?.active || 0} active`;
    document.getElementById('connections-total').textContent = `${data.connections?.total || 0} total`;
    
    const connectionsStatus = document.getElementById('connections-status');
    connectionsStatus.textContent = data.connections?.active > 0 ? 'Active' : 'Idle';
    connectionsStatus.className = 'font-medium text-status-green';

    // Requests
    const requestsPerMin = data.requests?.last_minute || 0;
    const requestsPercentage = Math.min(requestsPerMin / 100000 * 100, 100);
    updateProgressBar('requests-progress', requestsPercentage, 'bg-green-600');
    document.getElementById('requests-rate').textContent = `${requestsPerMin}/min`;
    document.getElementById('requests-total').textContent = `${data.requests?.total || 0} total`;
    
    const requestsStatus = document.getElementById('requests-status');
    requestsStatus.textContent = requestsPerMin > 0 ? 'Active' : 'Idle';
    requestsStatus.className = 'font-medium text-status-green';

    // Response Time
    const responseTime = data.requests?.average_response_time_ms || 0;
    const responsePercentage = Math.min(responseTime / 1000, 10); // max 10s
    updateProgressBar('response-progress', responsePercentage, 'bg-purple-600');
    document.getElementById('response-time').textContent = `${responseTime.toFixed(2)}ms`;
    document.getElementById('response-avg').textContent = `${responseTime.toFixed(2)}ms avg`;
    
    const responseStatus = document.getElementById('response-status');
    if (responseTime < 100) responseStatus.textContent = 'Fast';
    else if (responseTime < 500) responseStatus.textContent = 'Normal';
    else responseStatus.textContent = 'Slow';
    
    responseStatus.className = `font-medium ${responseTime < 500 ? 'text-status-green' : 'text-status-yellow'}`;
}

function updateSystemInfo(data) {
    document.getElementById('server-name').textContent = data.server?.name || 'WebServ';
    document.getElementById('server-version').textContent = data.server?.version || '1.0';
    document.getElementById('server-uptime').textContent = data.server_info?.uptime_formatted || '--';
    
    const serverStatus = document.getElementById('server-status');
    serverStatus.textContent = 'Online';
    serverStatus.className = 'font-mono text-gray-100 bg-green-600 w-fit rounded-full h-fit px-2 py-[2px] text-xs';
}

function updateDashboard(data) {
    updateServerMetrics(data);
	
    document.getElementById('server-uptime').textContent = data.server_info?.uptime_formatted || '--';
    lastUpdateTime = new Date();
    document.getElementById('last-update-time').textContent = lastUpdateTime.toLocaleTimeString();
}

function setupRefreshButton() {
    const refreshBtn = document.getElementById('refresh-btn');
    const refreshIcon = document.getElementById('refresh-icon');
    
    if (refreshBtn && refreshIcon) {
        refreshBtn.addEventListener('click', async () => {
            if (isRefreshing) return;
            
            isRefreshing = true;
            refreshIcon.classList.add('animate-spin');
            refreshBtn.disabled = true;
            
            try {
                await fetchStatus();
            } finally {
                setTimeout(() => {
                    isRefreshing = false;
                    refreshIcon.classList.remove('animate-spin');
                    refreshBtn.disabled = false;
                }, 1000);
            }
        });
    }
}

// Fetch status data
async function fetchStatus() {
    try {
        const response = await fetch(STATUS_ENDPOINT, {
            headers: {
                'Accept': 'application/json'
            }
        });

        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }

        const data = await response.json();
        updateDashboard(data);
        
    } catch (error) {
        console.error('Failed to fetch status:', error);
    }
}

function initDashboard() {
    setupRefreshButton();
    updateRefreshInterval();
    fetchStatus();
    setInterval(() => {
        fetchStatus();
    }, REFRESH_INTERVAL);
}

document.addEventListener('DOMContentLoaded', initDashboard);
