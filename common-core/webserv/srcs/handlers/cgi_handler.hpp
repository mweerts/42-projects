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

class ServerConfig;
struct CgiBin;
class CgiProcess;
class HttpRequest;
class HttpResponse;

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
	void setQueryString(const std::string& queryString);
    bool isProcessing() const;

	
    // Returns true if CGI completed (stdout EOF and child exited, or fatal).
    bool handleFdEvent(int fd, short revents);

    bool processCgiIO();
    void buildCgiResponse(HttpResponse& response);

   private:
    static const int CGI_TIMEOUT_SECONDS;

    const HttpRequest&  request_;
    const ServerConfig* serverConfig_;
    CgiProcess*         async_process_;
    static std::string  cgiScriptPath_;
	std::string   queryString_;

    // initialized at startup
    static std::map<std::string, std::string> cgiBin_;
    static std::string                        cgiBinPath_;
    static bool                               cgiBinInitialized_;

   private:
    const std::vector<std::string> buildEnvironment();

    std::string       getCgiInterpreter(const std::string& scriptPath);
    const std::string getFileExtension(const std::string& filePath);

    bool startCgiProcess(const std::string& scriptPath);

    std::string resolveScriptPath(const std::string& uri) const;
    void        setDefaultCgiBin();
};

#endif
