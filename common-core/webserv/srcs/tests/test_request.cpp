#include "HttpRequest.hpp"
#include <iostream>
#include <fstream>

// Example: Processing a POST request with JSON body
void handleJsonRequest(HttpRequest& request) {
    std::cout << "=== Processing JSON Request ===" << std::endl;
    
    // Check request details
    std::cout << "Method: " << request.getMethod() << std::endl;
    std::cout << "URI: " << request.getUri() << std::endl;
    std::cout << "Content-Type: " << request.getContentType() << std::endl;
    std::cout << "Body Size: " << request.getBodySize() << " bytes" << std::endl;
    
    // Process body (works the same for memory or file storage!)
    if (request.getBodySize() > 0) {
        std::cout << "Body content:" << std::endl;
        
        // Option 1: Read all at once (good for small bodies)
        std::string full_body = request.readBodyAll();
        std::cout << full_body << std::endl;
        
        // Option 2: Read in chunks (good for large bodies)
        request.resetBodyReader();
        std::string chunk;
        int chunk_count = 0;
        
        std::cout << "\nReading in chunks:" << std::endl;
        while (request.readBodyChunk(chunk, 50)) {  // 50 bytes at a time
            std::cout << "Chunk " << ++chunk_count << " (" << chunk.size() << " bytes): " 
                      << chunk << std::endl;
        }
    }
}

// Example: Processing a file upload
void handleFileUpload(HttpRequest& request) {
    std::cout << "\n=== Processing File Upload ===" << std::endl;
    
    std::cout << "Method: " << request.getMethod() << std::endl;
    std::cout << "URI: " << request.getUri() << std::endl;
    std::cout << "Content-Type: " << request.getContentType() << std::endl;
    std::cout << "Body Size: " << request.getBodySize() << " bytes" << std::endl;
    
    // For large file uploads, read in chunks to avoid memory issues
    if (request.getBodySize() > 1024) {  // > 1KB
        std::cout << "Large file detected, processing in chunks..." << std::endl;
        
        std::string chunk;
        size_t total_processed = 0;
        
		// chunk size is optional, default is 1024
        while (request.readBodyChunk(chunk, 1024)) {  
            // Process chunk (e.g., write to disk, validate, etc.)
            total_processed += chunk.size();
            std::cout << "Processed " << total_processed << " / " 
                      << request.getBodySize() << " bytes" << std::endl;
        }
    } else {
        // Small file, read all at once
        std::string file_content = request.readBodyAll();
        std::cout << "Small file content (" << file_content.size() << " bytes):" << std::endl;
        std::cout << file_content << std::endl;
    }
}

int main() {
    std::cout << "=== HttpRequest Usage Examples ===" << std::endl;
    
    // Example 1: Small JSON request (in-memory body)
    {
        HttpRequest request;
        request.setMethod("POST");
        request.setUri("/api/users");
        request.setHeader("Content-Type", "application/json");
        request.setBody("{\"name\":\"John\",\"email\":\"john@example.com\"}");
        
        handleJsonRequest(request);
    }
    
    {
        std::string filename = "large_upload.tmp";
        std::ofstream file(filename.c_str(), std::ios::binary);
        std::string large_content;
        
        // Create 5KB of data
        for (int i = 0; i < 500; ++i) {
            large_content += "0123456789";  // 10 bytes * 500 = 5KB
        }
        file.write(large_content.c_str(), large_content.length());
        file.close();
        
        HttpRequest request;
        request.setMethod("POST");
		request.setVersion("HTTP/1.1");
        request.setUri("/upload");
        request.setHeader("Content-Type", "application/octet-stream");
        request.setBodyFile(filename, 0, large_content.length());
        
		std::cout << request << std::endl;
        handleFileUpload(request);
        
        // Clean up
        std::remove(filename.c_str());
    }
    
	// not implemented yet
    // Example 3: Chunked encoding detection
    // {
    //     std::cout << "\n=== Chunked Encoding Example ===" << std::endl;
        
    //     HttpRequest request;
    //     request.setMethod("POST");
    //     request.setUri("/api/stream");
    //     request.setHeader("Transfer-Encoding", "chunked");
    //     request.setBody("This is chunked data");
        
    //     if (request.isRequestChunked()) {
    //         std::cout << "Request uses chunked encoding - special handling needed" << std::endl;
    //     }
        
    //     std::cout << "Body: " << request.readBodyAll() << std::endl;
    // }
    
    return 0;
} 
