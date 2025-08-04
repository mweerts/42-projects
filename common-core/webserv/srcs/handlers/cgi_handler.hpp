/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_handler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 20:35:53 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/01 20:35:54 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class ServerConfig;
struct CgiBin;

class CgiHandler {
   public:
    CgiHandler(const HttpRequest& request);
    ~CgiHandler();
    
    // Static initialization - call once at startup
    static void initializeCgiBin(const CgiBin& cgiBin);

    bool executeCgiScript(const std::string& scriptPath,
                          HttpResponse&      response);
    bool isCgiScript(const std::string& filePath);
    const std::map<std::string, std::string>& getCgiBin() const;

   private:
    const HttpRequest& request_;
    // extension -> interpreter path (static, shared across all instances)
    static std::map<std::string, std::string> cgiBin_;
	static std::string cgiBinPath_;
    static bool cgiBinInitialized_;

    // Environment setup
    std::vector<std::string> buildEnvironment();
    std::string              buildQueryString();

    // Process management
    bool createProcess(const std::string& scriptPath, int& childPid,
                       int& inputPipe, int& outputPipe);
    bool communicateWithProcess(int inputPipe, int outputPipe,
                                HttpResponse& response);

    bool parseCgiResponse(const std::string& cgiOutput, HttpResponse& response);
    std::string extractHeaders(const std::string& cgiOutput, std::string& body);

    std::string       getCgiInterpreter(const std::string& scriptPath);
    const std::string getFileExtension(const std::string& filePath);
    void              cleanupProcess(int pid);

    // Timeout handling
    static const int CGI_TIMEOUT_SECONDS = 30;
    bool             waitForProcess(int pid);

    void setDefaultCgiBin();
};

#endif
