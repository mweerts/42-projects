/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:44:01 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/01 20:44:02 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi_handler.hpp"

#include <signal.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <set>
#include <sstream>

#include "../parsing/GlobalConfig.hpp"
#include "Logger.hpp"
#include "lib/file_utils.hpp"

CgiHandler::CgiHandler(const HttpRequest& request) : request_(request) {
    setDefaultCgiBin();
}

CgiHandler::~CgiHandler() {}

void CgiHandler::setDefaultCgiBin() {
    cgiBin_["py"] = "/usr/bin/python3";
    cgiBin_["sh"] = "/bin/bash";
    cgiBin_["php"] = "/usr/bin/php-cgi";
}

bool CgiHandler::loadCgiBin(const CgiBin& cgiBin) {
    cgiBin_.clear();
    // std::vector<std::string> extensions = cgiBin.getExt();
    // std::vector<std::string> paths = cgiBin.getPath();

	(void)cgiBin;
    // if (extensions.empty() || paths.empty()) {
    //     Logger::error() << "No extensions or interpreters found in cgiBin";
    //     setDefaultCgiBin(); // TEMP
    //     return true;
    // }

    // for (size_t i = 0; i < extensions.size(); i++) {
    //     cgiBin_[extensions[i]] = paths[i];  // extension -> path
    // }
    return true;
}

const std::map<std::string, std::string>& CgiHandler::getCgiBin() const {
    return cgiBin_;
}

bool CgiHandler::isCgiScript(const std::string& filePath) {
	Logger::debug() << "isCgiScript: " << filePath;
    if (!lib::pathExist(filePath) || !lib::isExecutable(filePath)) {
        return false;
    }

    std::string ext = getFileExtension(filePath);
    std::map<std::string, std::string>::const_iterator it = cgiBin_.find(ext);
    if (it == cgiBin_.end()) {
        return false;
    }

    return lib::isExecutable(it->second);
}

const std::string CgiHandler::getFileExtension(const std::string& filePath) {
    size_t lastDot = filePath.find_last_of('.');
    if (lastDot == std::string::npos) {
        return "";
    }
    return filePath.substr(lastDot + 1);
}

// // Step 4: Get appropriate interpreter for script
// std::string CgiHandler::getCgiInterpreter(const std::string& scriptPath) {
//     std::string extension = getFileExtension(scriptPath);

//     // Look up interpreter in our supported extensions map
//     std::map<std::string, std::string>::const_iterator it =
//         _cgiBin.find(extension);
//     if (it != _cgiBin.end()) {
//         Logger::debug() << "Found interpreter for " << extension << ": "
//                         << it->second;
//         return it->second;
//     }

//     // Extension not supported
//     Logger::debug() << "Extension " << extension << " not supported for CGI";
//     return "";
// }

// // Step 5: Build query string from request
// std::string CgiHandler::buildQueryString() {
//     std::string uri = request_.getUri();
//     size_t      queryPos = uri.find('?');
//     if (queryPos == std::string::npos) {
//         return "";
//     }
//     return uri.substr(queryPos + 1);
// }

// // Step 6: Build environment variables for CGI script
// std::vector<std::string> CgiHandler::buildEnvironment() {
//     std::vector<std::string> env;

//     // Standard CGI environment variables
//     env.push_back("REQUEST_METHOD=" + request_.getMethod());
//     env.push_back("QUERY_STRING=" + buildQueryString());
//     env.push_back("CONTENT_LENGTH=" +
//                   std::to_string(request_.getBody().length()));
//     env.push_back("CONTENT_TYPE=" + request_.getContentType());
//     env.push_back("SCRIPT_NAME=" + request_.getUri());
//     env.push_back("PATH_INFO=" + request_.getUri());  // Full path as
//     requested env.push_back("SERVER_NAME=localhost");
//     env.push_back("SERVER_PORT=8080");  // Default port
//     env.push_back("SERVER_PROTOCOL=HTTP/1.1");
//     env.push_back("SERVER_SOFTWARE=webserv/1.0");

//     // HTTP headers as environment variables
//     const std::map<std::string, std::string>& headers =
//     request_.getHeaders(); for (std::map<std::string,
//     std::string>::const_iterator it =
//              headers.begin();
//          it != headers.end(); ++it) {
//         std::string envName = "HTTP_" + it->first;
//         std::transform(envName.begin(), envName.end(), envName.begin(),
//                        ::toupper);
//         std::replace(envName.begin(), envName.end(), '-', '_');
//         env.push_back(envName + "=" + it->second);
//     }

//     Logger::debug() << "Built " << env.size()
//                     << " environment variables for CGI";
//     return env;
// }

// // Step 7: Create process and set up communication
// bool CgiHandler::createProcess(const std::string& scriptPath, int& childPid,
//                                int& inputPipe, int& outputPipe) {
//     // Create pipes for communication
//     int inputPipeFds[2];
//     int outputPipeFds[2];

//     if (pipe(inputPipeFds) == -1 || pipe(outputPipeFds) == -1) {
//         Logger::error() << "Failed to create pipes for CGI process";
//         return false;
//     }

//     // Fork the process
//     childPid = fork();
//     if (childPid == -1) {
//         Logger::error() << "Failed to fork CGI process";
//         close(inputPipeFds[0]);
//         close(inputPipeFds[1]);
//         close(outputPipeFds[0]);
//         close(outputPipeFds[1]);
//         return false;
//     }

//     if (childPid == 0) {
//         // Child process - execute CGI script
//         try {
//             // Close unused pipe ends
//             close(inputPipeFds[1]);   // Close write end of input pipe
//             close(outputPipeFds[0]);  // Close read end of output pipe

//             // Redirect stdin/stdout
//             dup2(inputPipeFds[0], STDIN_FILENO);
//             dup2(outputPipeFds[1], STDOUT_FILENO);

//             // Close original pipe ends
//             close(inputPipeFds[0]);
//             close(outputPipeFds[1]);

//             // Change to script directory (requirement 8)
//             std::string scriptDir =
//                 scriptPath.substr(0, scriptPath.find_last_of('/'));
//             if (!scriptDir.empty() && chdir(scriptDir.c_str()) != 0) {
//                 Logger::error()
//                     << "Failed to change directory to: " << scriptDir;
//             }

//             // Prepare environment
//             std::vector<std::string> envVars = buildEnvironment();
//             std::vector<const char*> envArray;
//             for (std::vector<std::string>::iterator it = envVars.begin();
//                  it != envVars.end(); ++it) {
//                 envArray.push_back(it->c_str());
//             }
//             envArray.push_back(NULL);  // NULL terminator

//             // Get interpreter
//             std::string interpreter = getCgiInterpreter(scriptPath);

//             // Prepare arguments (requirement 7: file as first argument)
//             std::vector<const char*> args;
//             if (!interpreter.empty()) {
//                 args.push_back(interpreter.c_str());
//             }
//             args.push_back(
//                 scriptPath.c_str());  // Script path as first argument
//             args.push_back(NULL);     // NULL terminator

//             // Execute the script
//             if (!interpreter.empty()) {
//                 execve(interpreter.c_str(),
//                        const_cast<char* const*>(args.data()),
//                        const_cast<char* const*>(envArray.data()));
//             } else {
//                 execve(scriptPath.c_str(),
//                        const_cast<char* const*>(args.data()),
//                        const_cast<char* const*>(envArray.data()));
//             }

//             // If we get here, exec failed
//             Logger::error() << "Failed to execute CGI script: " <<
//             scriptPath; exit(1);

//         } catch (const std::exception& e) {
//             Logger::error() << "Exception in CGI child process: " <<
//             e.what(); exit(1);
//         }
//     } else {
//         // Parent process - set up communication
//         close(inputPipeFds[0]);   // Close read end of input pipe
//         close(outputPipeFds[1]);  // Close write end of output pipe

//         inputPipe = inputPipeFds[1];    // Write to child's stdin
//         outputPipe = outputPipeFds[0];  // Read from child's stdout

//         return true;
//     }
// }

// // Step 8: Communicate with the CGI process
// bool CgiHandler::communicateWithProcess(int inputPipe, int outputPipe,
//                                         HttpResponse& response) {
//     // Send request body to CGI script via stdin (requirement 5: handle
//     chunked) std::string requestBody = request_.getBody(); if
//     (!requestBody.empty()) {
//         ssize_t written =
//             write(inputPipe, requestBody.c_str(), requestBody.length());
//         if (written == -1) {
//             Logger::error() << "Failed to write to CGI process stdin";
//             close(inputPipe);
//             close(outputPipe);
//             return false;
//         }
//     }
//     close(inputPipe);  // Close stdin to signal EOF (requirement 5)

//     // Read output from CGI script (requirement 6: handle missing
//     // content-length)
//     std::string cgiOutput;
//     char        buffer[4096];
//     ssize_t     bytesRead;

//     while ((bytesRead = read(outputPipe, buffer, sizeof(buffer))) > 0) {
//         cgiOutput.append(buffer, bytesRead);
//     }

//     close(outputPipe);

//     Logger::debug() << "Read " << cgiOutput.length()
//                     << " bytes from CGI script";

//     // Parse CGI response
//     return parseCgiResponse(cgiOutput, response);
// }

// // Step 9: Parse CGI response
// bool CgiHandler::parseCgiResponse(const std::string& cgiOutput,
//                                   HttpResponse&      response) {
//     if (cgiOutput.empty()) {
//         Logger::error() << "Empty response from CGI script";
//         return false;
//     }

//     // Split headers and body
//     std::string headers, body;
//     size_t      headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         headerEnd = cgiOutput.find("\n\n");
//     }

//     if (headerEnd != std::string::npos) {
//         headers = cgiOutput.substr(0, headerEnd);
//         body = cgiOutput.substr(
//             headerEnd +
//             (cgiOutput.find("\r\n\r\n") != std::string::npos ? 4 : 2));
//     } else {
//         // No headers found, treat entire output as body
//         body = cgiOutput;
//     }

//     // Parse headers
//     std::istringstream headerStream(headers);
//     std::string        line;
//     bool               hasStatus = false;

//     while (std::getline(headerStream, line)) {
//         if (line.empty() || line == "\r")
//             continue;

//         // Remove carriage return if present
//         if (!line.empty() && line.back() == '\r') {
//             line.pop_back();
//         }

//         size_t colonPos = line.find(':');
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 1);

//             // Trim whitespace from value
//             while (!value.empty() && value[0] == ' ') {
//                 value.erase(0, 1);
//             }

//             if (key == "Status") {
//                 // Parse status line (e.g., "Status: 200 OK")
//                 size_t spacePos = value.find(' ');
//                 if (spacePos != std::string::npos) {
//                     std::string statusCode = value.substr(0, spacePos);
//                     response.setStatusCode(
//                         static_cast<StatusCode>(std::atoi(statusCode.c_str())));
//                 }
//                 hasStatus = true;
//             } else if (key == "Content-Type") {
//                 response.setContentType(value);
//             } else if (key == "Location") {
//                 response.setHeader("Location", value);
//             } else {
//                 response.setHeader(key, value);
//             }
//         }
//     }

//     // Set default status if not provided
//     if (!hasStatus) {
//         response.setStatusCode(HTTP_OK);
//     }

//     // Set content
//     response.setContent(body);

//     Logger::debug() << "Parsed CGI response with " << body.length()
//                     << " bytes of content";
//     return true;
// }

// // Step 10: Wait for process with timeout
// bool CgiHandler::waitForProcess(int pid) {
//     int    status;
//     time_t startTime = time(NULL);

//     while (time(NULL) - startTime < CGI_TIMEOUT_SECONDS) {
//         int result = waitpid(pid, &status, WNOHANG);
//         if (result == pid) {
//             // Process finished
//             if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
//                 return true;
//             } else {
//                 Logger::error() << "CGI script exited with error code: "
//                                 << WEXITSTATUS(status);
//                 return false;
//             }
//         } else if (result == -1) {
//             Logger::error() << "Error waiting for CGI process";
//             return false;
//         }
//         // Process still running, wait a bit
//         usleep(10000);  // 10ms
//     }

//     // Timeout reached
//     Logger::error() << "CGI script timed out after " << CGI_TIMEOUT_SECONDS
//                     << " seconds";
//     kill(pid, SIGKILL);
//     waitpid(pid, &status, 0);
//     return false;
// }

// // Step 11: Clean up process
// void CgiHandler::cleanupProcess(int pid) {
//     int status;
//     if (waitpid(pid, &status, WNOHANG) == 0) {
//         // Process still running, kill it
//         kill(pid, SIGKILL);
//         waitpid(pid, &status, 0);
//     }
// }

// // Step 12: Main CGI execution method
// bool CgiHandler::executeCgiScript(const std::string& scriptPath,
//                                   HttpResponse&      response) {
//     Logger::debug() << "Executing CGI script: " << scriptPath;

//     int childPid, inputPipe, outputPipe;

//     // Create process
//     if (!createProcess(scriptPath, childPid, inputPipe, outputPipe)) {
//         Logger::error() << "Failed to create CGI process";
//         return false;
//     }

//     // Communicate with process
//     bool success = communicateWithProcess(inputPipe, outputPipe, response);

//     // Wait for process to finish
//     if (!waitForProcess(childPid)) {
//         Logger::error() << "CGI process failed or timed out";
//         cleanupProcess(childPid);
//         return false;
//     }

//     cleanupProcess(childPid);

//     if (success) {
//         Logger::debug() << "CGI script executed successfully";
//     } else {
//         Logger::error() << "Failed to process CGI output";
//     }

//     return success;
// }
