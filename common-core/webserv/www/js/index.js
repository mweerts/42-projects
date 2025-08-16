
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
                // setTimeout(() => {
                //     refreshFileList();
                //     statusDiv.classList.add("hidden");
                // }, 2000);
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

// Display files in terminal style
function displayFiles(files) {
    const fileListDiv = document.getElementById("file-list");

    if (files.length === 0) {
        fileListDiv.innerHTML =
            '<div class="text-yellow-400">No files uploaded yet</div>';
        return;
    }

    let html = "";
    files.forEach((file) => {
        html += `
        <div class="flex justify-between items-center py-1 hover:bg-green-900 hover:bg-opacity-20 px-2 -mx-2">
            <span class="text-cyan-300">${file.name}</span>
            <span class="text-pink-300 text-xs">${file.size} bytes</span>
        </div>
    `;
    });

    fileListDiv.innerHTML = html;
}

// Initial file list load
// refreshFileList();

// Connection counter and uptime
let connectionCount = 0;
let startTime = Date.now();

function updateStats() {
    connectionCount = Math.floor(Math.random() * 5);
    document.getElementById("connections").textContent = connectionCount;

    const elapsed = Date.now() - startTime;
    const hours = Math.floor(elapsed / 3600000);
    const minutes = Math.floor((elapsed % 3600000) / 60000);
    const seconds = Math.floor((elapsed % 60000) / 1000);
    document.getElementById("uptime").textContent =
        `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
}

setInterval(updateStats, 1000);
updateStats();
