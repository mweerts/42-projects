// Configuration
const STATUS_ENDPOINT = '/status';
const REFRESH_INTERVAL = 500;
let lastUpdateTime = new Date();

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
    // Connections
    const connectionPercentage = Math.min((data.connections?.active || 0) / 1000 * 100, 100);
    updateProgressBar('connections-progress', connectionPercentage, 'bg-cyan-400');
    document.getElementById('connections-usage').textContent = `${Math.round(connectionPercentage)}%`;
    document.getElementById('connections-active').textContent = `${data.connections?.active || 0} active`;
    document.getElementById('connections-total').textContent = `${data.connections?.total || 0} total`;
    
    const connectionsStatus = document.getElementById('connections-status');
    connectionsStatus.textContent = connectionPercentage < 50 ? 'Healthy' : connectionPercentage < 80 ? 'Warning' : 'Critical';
    connectionsStatus.className = `font-medium ${connectionPercentage < 50 ? 'text-green-400' : connectionPercentage < 80 ? 'text-yellow-400' : 'text-red-400'}`;

    // Requests
    const requestsPerMin = data.requests?.last_minute || 0;
    const requestsPercentage = Math.min(requestsPerMin / 10000 * 100, 100);
    updateProgressBar('requests-progress', requestsPercentage, 'bg-pink-400');
    document.getElementById('requests-rate').textContent = `${requestsPerMin}/min`;
    document.getElementById('requests-total').textContent = `${data.requests?.total || 0} total`;
    
    const requestsStatus = document.getElementById('requests-status');
    requestsStatus.textContent = requestsPerMin > 0 ? 'Active' : 'Idle';
    requestsStatus.className = 'font-medium text-green-400';

    // Response Time
    const responseTime = data.requests?.average_response_time_ms || 0;
    const responsePercentage = Math.min(responseTime / 100, 100);
    updateProgressBar('response-progress', responsePercentage, 'bg-purple-400');
    document.getElementById('response-time').textContent = `${responseTime.toFixed(1)}ms`;
    document.getElementById('response-avg').textContent = `${responseTime.toFixed(1)} ms avg`;
    
    const responseStatus = document.getElementById('response-status');
    if (responseTime < 100) responseStatus.textContent = 'Fast';
    else if (responseTime < 500) responseStatus.textContent = 'Normal';
    else responseStatus.textContent = 'Slow';
    
    responseStatus.className = `font-medium ${responseTime < 500 ? 'text-green-400' : 'text-yellow-400'}`;
}

async function fetchStatus() {
    try {
        const response = await fetch(STATUS_ENDPOINT);
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }

        const data = await response.json();
        updateServerMetrics(data);
        
    } catch (error) {
        console.error('Failed to fetch status:', error);
        showFallbackData();
    }
}

function showFallbackData() {
    const demoData = {
        connections: { active: 5, total: 1000 },
        requests: { last_minute: 12, total: 1500, average_response_time_ms: 45 }
    };
    updateServerMetrics(demoData);
}

function initStatusDashboard() {
    fetchStatus();
    setInterval(fetchStatus, REFRESH_INTERVAL);
}

window.initStatusDashboard = initStatusDashboard;
