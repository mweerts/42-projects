
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

async function fetchStats() {
    try {
		const response = await fetch("/status", {
			headers: {
				'Accept': 'application/json'
			}
		});

		if (!response.ok) {
			throw new Error(`HTTP ${response.status}`);
		}

   		const data = await response.json();
		document.getElementById("connections").textContent = data.connections?.active || 0;

		const uptime = data.server_info?.uptime_formatted || '--';
    	document.getElementById("uptime").textContent = uptime;

	} catch (error) {
		console.error("Failed to fetch stats:", error);
	}
}

function updateStats() {
    fetchStats();
    
    setInterval(() => {
        fetchStats();
    }, 600);
}

document.addEventListener("DOMContentLoaded", updateStats);
