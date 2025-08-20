// Handles drag & drop, file selection, uploads, and file listing

// DOM elements
const dropZone = document.getElementById('drop-zone');
const fileInput = document.getElementById('file-input');
const selectedFilesDiv = document.getElementById('selected-files');
const filesList = document.getElementById('files-list');

// Drag and drop functionality
dropZone.addEventListener('dragover', (e) => {
    e.preventDefault();
    dropZone.classList.add('border-pink-400', 'bg-pink-400/10');
});

dropZone.addEventListener('dragleave', (e) => {
    e.preventDefault();
    if (!dropZone.contains(e.relatedTarget)) {
        dropZone.classList.remove('border-pink-400', 'bg-pink-400/10');
    }
});

dropZone.addEventListener('drop', (e) => {
    e.preventDefault();
    dropZone.classList.remove('border-pink-400', 'bg-pink-400/10');
    const files = Array.from(e.dataTransfer.files);
    handleFiles(files);
});

// Click handler for drop zone
dropZone.addEventListener('click', (e) => {
    // Add visual feedback
    dropZone.classList.add('border-pink-400', 'bg-pink-400/10');
    setTimeout(() => {
        dropZone.classList.remove('border-pink-400', 'bg-pink-400/10');
    }, 200);
    
    fileInput.click();
});

fileInput.addEventListener('change', (e) => {
    const files = Array.from(e.target.files);
    handleFiles(files);
});

function handleFiles(files) {
    if (files.length === 0) return;
    
    const dt = new DataTransfer();
    files.forEach(file => dt.items.add(file));
    fileInput.files = dt.files;
    
    displaySelectedFiles(files);
}

function displaySelectedFiles(files) {
    if (files.length === 0) {
        selectedFilesDiv.classList.add('hidden');
        return;
    }

    selectedFilesDiv.classList.remove('hidden');
    filesList.innerHTML = files.map(file => `
        <div class="flex justify-between items-center py-2 px-3 bg-gradient-to-r from-pink-400/10 to-purple-400/5 border border-pink-400/30 rounded-lg text-xs hover:border-pink-400/50 hover:bg-gradient-to-r hover:from-pink-400/15 hover:to-purple-400/10 transition-all duration-300 group">
            <span class="text-pink-300 font-mono font-semibold group-hover:text-pink-200 transition-colors duration-300">${file.name}</span>
            <span class="text-gray-400 group-hover:text-gray-300 transition-colors duration-300">${formatFileSize(file.size)}</span>
        </div>
    `).join('');
}

function formatFileSize(bytes) {
    if (bytes === 0) return '0 B';
    const k = 1024;
    const sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(1)) + ' ' + sizes[i];
}

// File upload handling
document
    .getElementById("upload-form")
    .addEventListener("submit", async function (e) {
        e.preventDefault();

        const statusDiv = document.getElementById("upload-status");
        const messageSpan =
            document.getElementById("upload-message");

        statusDiv.classList.remove("hidden");
        messageSpan.textContent = "Uploading...";

        const formData = new FormData(this);

        try {
            const response = await fetch("/upload", {
                method: "POST",
                body: formData,
            });

            if (response.ok) {
                messageSpan.textContent = "Upload successful!";
                statusDiv.className = statusDiv.className.replace(
                    "border-green-500",
                    "border-green-400",
                );

                // Clear selected files after successful upload
                selectedFilesDiv.classList.add('hidden');
                fileInput.value = '';
                refreshFileList();
                setTimeout(() => {
                    statusDiv.classList.add("hidden");
                }, 5000);
            } else {
                messageSpan.textContent = `Upload failed: ${response.status}`;
                statusDiv.className = statusDiv.className.replace(
                    "border-green-500",
                    "border-red-500",
                );
            }
        } catch (error) {
            messageSpan.textContent = `Upload error: ${error.message}`;
            statusDiv.className = statusDiv.className.replace(
                "border-green-500",
                "border-red-500",
            );
        }
    });

async function refreshFileList() {
    const fileListDiv = document.getElementById("file-list");
    fileListDiv.innerHTML =
        '<div class="animate-pulse">Scanning file system...</div>';

    try {
        const response = await fetch("/list-files.py");
        if (response.ok) {
            const data = await response.json();
            if (data.success) {
                displayFiles(data.files);
            } else {
                fileListDiv.innerHTML =
                    '<div class="text-red-400">Error: ' + (data.error || 'Unknown error') + '</div>';
            }
        } else {
            fileListDiv.innerHTML =
                '<div class="text-red-400">Error loading files (HTTP ' + response.status + ')</div>';
        }
    } catch (error) {
        fileListDiv.innerHTML =
            '<div class="text-red-400">Connection error: ' + error.message + '</div>';
    }
}

// Display files in terminal style
function displayFiles(files) {
    const fileListDiv = document.getElementById("file-list");

    let html = '<div class="animate-pulse text-green-400 mb-2">';
    html += '<span class="text-green-500">></span> Scanning filesystem...</div>';

    if (files.length === 0) {
        html += '<div class="text-yellow-400">No files uploaded yet</div>';
    } else {
        files.forEach((file) => {
            const size = formatFileSize(file.size);
            const date = new Date(file.modified).toLocaleDateString();
            html += `
            <div class="flex justify-between items-center py-1 hover:bg-green-900 hover:bg-opacity-20 px-2 -mx-2">
                <span class="text-cyan-300">${file.name}</span>
                <div class="flex items-center space-x-4">
                    <span class="text-pink-300 text-xs">${size}</span>
                    <span class="text-gray-400 text-xs">${date}</span>
                </div>
            </div>
        `;
        });
    }

    fileListDiv.innerHTML = html;
}

refreshFileList();
