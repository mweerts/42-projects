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
class CgiProcess;

class CgiHandler {
   public:
    CgiHandler(const HttpRequest& request);
    CgiHandler(const HttpRequest& request, const ServerConfig* serverConfig);
    ~CgiHandler();

    // to call at server startup
    static void initializeCgiBin(const CgiBin& cgiBin);

    bool isCgiScript(const std::string& uri);
    const std::map<std::string, std::string>& getCgiBin() const;

    bool startAsyncCgi(const std::string& scriptPath);
    bool isAsyncCgiComplete() const;
    void cleanupAsyncCgi();

	bool isTimedOut() const;

    int  getInputPipe() const;
    int  getOutputPipe() const;
    bool isProcessing() const;

    // Async processing methods
    bool processCgiIO();
    void buildCgiResponse(HttpResponse& response);

   private:
    static const int CGI_TIMEOUT_SECONDS;

    const HttpRequest& request_;
	const ServerConfig* serverConfig_;
    CgiProcess*        async_process_;
    static std::string cgiScriptPath_;

    // initialized at startup
    static std::map<std::string, std::string> cgiBin_;
    static std::string                        cgiBinPath_;
    static bool                               cgiBinInitialized_;


   private:
    // Environment setup
    const std::vector<std::string> buildEnvironment();
    std::string                    buildQueryString();


    // bool parseCgiResponse(const std::string& cgiOutput, HttpResponse& response);
    // std::string extractHeaders(const std::string& cgiOutput, std::string& body);

    std::string       getCgiInterpreter(const std::string& scriptPath);
    const std::string getFileExtension(const std::string& filePath);
    // void              cleanupProcess(int pid);

    bool startCgiProcess(const std::string& scriptPath);

    std::string resolveScriptPath(const std::string& uri) const;
    void        setDefaultCgiBin();

};

#endif
