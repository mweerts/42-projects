package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	// CGI environment variables
	remoteAddr := os.Getenv("REMOTE_ADDR")
	if remoteAddr == "" {
		remoteAddr = "unknown"
	}
	
	userAgent := os.Getenv("HTTP_USER_AGENT")
	if userAgent == "" {
		userAgent = "unknown"
	}

	// Output HTTP headers
	fmt.Println("Content-Type: text/html; charset=utf-8")
	fmt.Println("Cache-Control: no-cache")
	fmt.Println("") // Empty line separates headers from body
	
	// Output HTML content
	fmt.Println("<!DOCTYPE html>")
	fmt.Println("<html>")
	fmt.Println("<head>")
	fmt.Println("    <title>Go CGI Test</title>")
	fmt.Println("    <meta charset=\"utf-8\">")
	fmt.Println("</head>")
	fmt.Println("<body>")
	fmt.Println("    <h1>Go CGI Test - Success!</h1>")
	fmt.Println("    <p>If you can see this, your Go CGI is working correctly.</p>")
	fmt.Println("    <h2>CGI Environment Information:</h2>")
	fmt.Println("    <ul>")
	fmt.Printf("        <li><strong>Remote Address:</strong> %s</li>\n", remoteAddr)
	fmt.Printf("        <li><strong>User Agent:</strong> %s</li>\n", userAgent)
	fmt.Printf("        <li><strong>Server Time:</strong> %s</li>\n", time.Now().Format("2006-01-02 15:04:05"))
	fmt.Println("    </ul>")
	fmt.Println("    <h2>All CGI Environment Variables:</h2>")
	fmt.Println("    <ul>")
	
	// List all environment variables
	for _, env := range os.Environ() {
		fmt.Printf("        <li><code>%s</code></li>\n", env)
	}
	
	fmt.Println("    </ul>")
	fmt.Println("</body>")
	fmt.Println("</html>")
}
