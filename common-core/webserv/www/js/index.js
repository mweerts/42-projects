let isInitialized = false;
let configData = null;

function initHomepage() {
    if (isInitialized) return;
    isInitialized = true;
    
    fetchConfig();
    fetchStatus();
    setInterval(fetchStatus, 500);
}

async function fetchConfig() {
    try {
        const response = await fetch('/config');
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        configData = await response.json();
        updateServerInfo(configData);
        updateLocations(configData.locations || []);
        
    } catch (error) {
        console.error('Failed to fetch configuration:', error);
    }
}

function updateDashboard(data) {
    updateServerInfo(data);
}

function updateServerInfo(data) {
    const elements = {
        'server-name': data.server_name || 'WebServ',
        'server-port': data.port || '8080',
        'server-root': data.root || '/www/',
        'server-index': data.index || 'index.html',
        'server-autoindex': data.autoindex || 'off',
		'upload-dir': data.upload_dir || 'uploads',
    };
    
    Object.entries(elements).forEach(([id, value]) => {
        const element = document.getElementById(id);
        if (element) {
            element.textContent = value;
        }
    });
}

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
    
	// Format client_max_body_size (number of bytes) to a human-readable string (e.g., 10M, 1G)
	function formatBytes(bytes) {
		if (typeof bytes !== 'number' || isNaN(bytes)) return 'N/A';
		const units = ['B', 'K', 'M', 'G', 'T'];
		let i = 0;
		let value = bytes;
		while (value >= 1024 && i < units.length - 1) {
			value /= 1024;
			i++;
		}
		// Show no decimals for K/M/G/T, only for B
		const formatted = i === 0 ? value : Math.round(value);
		return formatted + units[i];
	}
	const clientMaxBodySize = location.client_max_body_size ? formatBytes(Number(location.client_max_body_size)) : '100M';
	
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
            
			<div class="flex items-center justify-between">
                <span class="text-gray-400">Client Max Body Size:</span>
                <span class="font-mono">${clientMaxBodySize}</span>
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

document.addEventListener('DOMContentLoaded', initHomepage);
