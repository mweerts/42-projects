/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_process.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:42:20 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/05 15:42:21 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_PROCESS_HPP
#define CGI_PROCESS_HPP

#include <sys/time.h>

#include <string>

class CgiProcess {
   public:
    CgiProcess()
        : childPid(-1),
          inputPipe(-1),
          outputPipe(-1),
          startTime(0),
          isValid(false) {}

    ~CgiProcess() {
        cleanup();
    }

    CgiProcess(const CgiProcess&) {};
    CgiProcess& operator=(const CgiProcess&) {};

    int         childPid;
    int         inputPipe;
    int         outputPipe;
    std::string outputBuffer;  // Accumulate CGI output
    std::string inputBuffer;   // Data to send to CGI
    time_t      startTime;
    bool        isValid;

    void cleanup();
    // void start(const std::string& scriptPath, const std::string& queryString);
    // void handleInput();
    // void handleOutput();
    // void handleTimeout();
    // void handleSignal(int signum);
    // void handleChildExit(int status);
};

#endif
