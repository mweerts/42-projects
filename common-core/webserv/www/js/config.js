// Configuration constants
const REFRESH_INTERVAL = 5000; // 5 seconds
let configData = null;
let lastUpdateTime = null;
let isRefreshing = false;

// Initialize the configuration dashboard
document.addEventListener('DOMContentLoaded', function() {
    initDashboard();
});

// Delay the auto-refresh to start after initial load
function initDashboard() {
    cacheElements();
    setupRefreshButton();
    updateRefreshInterval();
    fetchConfig();
    
    // Delay auto-refresh to start after 10 seconds instead of immediately
    setTimeout(() => {
        setInterval(fetchConfig, REFRESH_INTERVAL);
    }, 10000);
}

// Cache DOM elements to avoid repeated lookups
let cachedElements = {};

function cacheElements() {
    cachedElements = {
        'server-name': document.getElementById('server-name'),
        'server-port': document.getElementById('server-port'),
        'server-root': document.getElementById('server-root'),
        'server-index': document.getElementById('server-index'),
        'server-autoindex': document.getElementById('server-autoindex'),
        'locations-container': document.getElementById('locations-container'),
        'last-update-time': document.getElementById('last-update-time')
    };
}

function updateRefreshInterval() {
    const element = document.getElementById('refresh-interval');
    if (element) {
        element.textContent = `Every ${REFRESH_INTERVAL}ms`;
    }
}

// Fetch configuration data from the server
async function fetchConfig() {
    try {
        const response = await fetch('/config');
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        configData = await response.json();
        updateDashboard(configData);
        updateLastUpdateTime();
        
    } catch (error) {
        console.error('Failed to fetch configuration:', error);
        showError('Failed to load configuration data');
    }
}

// Update the dashboard with configuration data
function updateDashboard(data) {
    updateServerInfo(data);
    updateLocations(data.locations || []);
    updateConfigJson(data);
}

// Update server information section
function updateServerInfo(data) {
    const elements = {
        'server-name': data.server_name || 'WebServ',
        'server-port': data.port || '8080',
        'server-root': data.root || '/www/',
        'server-index': data.index || 'index.html',
        'server-autoindex': data.autoindex || 'off'
    };
    
    Object.entries(elements).forEach(([id, value]) => {
        const element = document.getElementById(id);
        if (element) {
            element.textContent = value;
        }
    });
}

// Update locations section with dynamic cards
function updateLocations(locations) {
    const container = document.getElementById('locations-container');
    if (!container) return;
    
    container.innerHTML = '';
    
    if (locations.length === 0) {
        container.innerHTML = `
            <div class="col-span-full text-center text-gray-500 font-mono">
                No location routes configured
            </div>
        `;
        return;
    }
    
    locations.forEach((location, index) => {
        const card = createLocationCard(location, index);
        container.appendChild(card);
    });
}

// Create a location card element
function createLocationCard(location, index) {
    const card = document.createElement('div');
    card.className = 'border border-pink-400/30 bg-black/20 p-4 rounded-lg hover:border-pink-400/60 transition-all duration-300';
    
    const methods = location.methods || [];
    const methodColors = {
        'GET': 'text-green-400',
        'POST': 'text-blue-400',
        'DELETE': 'text-red-400',
        'PUT': 'text-yellow-400',
        'PATCH': 'text-purple-400'
    };
    
    const autoindexStatus = location.autoindex === 'on' ? 
        '<span class="text-green-400">✓ ON</span>' : 
        '<span class="text-red-400">✗ OFF</span>';
    
    card.innerHTML = `
        <div class="flex items-center justify-between mb-3">
            <h3 class="font-mono font-bold text-pink-400 text-lg">${location.path}</h3>
            <span class="text-xs text-gray-500">#${index + 1}</span>
        </div>
        
        <div class="space-y-2 text-sm">
            <div class="flex items-center justify-between">
                <span class="text-gray-400">Root:</span>
                <span class="font-mono text-white">${location.root || '/'}</span>
            </div>
            
            <div class="flex items-center justify-between">
                <span class="text-gray-400">Index:</span>
                <span class="font-mono text-white">${location.index || 'index.html'}</span>
            </div>
            
            <div class="flex items-center justify-between">
                <span class="text-gray-400">Autoindex:</span>
                <span class="font-mono">${autoindexStatus}</span>
            </div>
            
            ${location.return ? `
            <div class="flex items-center justify-between">
                <span class="text-gray-400">Return:</span>
                <span class="font-mono text-yellow-400">${location.return}</span>
            </div>
            ` : ''}
            
            ${location.alias ? `
            <div class="flex items-center justify-between">
                <span class="text-gray-400">Alias:</span>
                <span class="font-mono text-cyan-400">${location.alias}</span>
            </div>
            ` : ''}
        </div>
        
        <div class="mt-4 pt-3 border-t border-pink-400/20">
            <div class="text-xs text-gray-400 mb-2">HTTP Methods:</div>
            <div class="flex flex-wrap gap-2">
                ${methods.map(method => `
                    <span class="px-2 py-1 bg-black/40 border border-current rounded text-xs font-mono ${methodColors[method] || 'text-gray-400'}">
                        ${method}
                    </span>
                `).join('')}
            </div>
        </div>
    `;
    
    return card;
}

// Update the JSON configuration display
function updateConfigJson(data) {
    const jsonDisplay = document.getElementById('json-display');
    if (jsonDisplay) {
        jsonDisplay.textContent = JSON.stringify(data, null, 2);
    }
}

// Update last update time
function updateLastUpdateTime() {
    lastUpdateTime = new Date();
    const element = document.getElementById('last-update-time');
    if (element) {
        element.textContent = lastUpdateTime.toLocaleTimeString();
    }
}

// Setup refresh button functionality
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
                await fetchConfig();
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

// Show error message
function showError(message) {
    const errorDiv = document.createElement('div');
    errorDiv.className = 'fixed top-4 right-4 bg-red-600 text-white px-6 py-3 rounded-lg font-mono z-50';
    errorDiv.textContent = `ERROR: ${message}`;
    
    document.body.appendChild(errorDiv);
    
    setTimeout(() => {
        errorDiv.remove();
    }, 5000);
}
