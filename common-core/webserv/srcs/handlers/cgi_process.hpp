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
#include <vector>

class CgiProcess {
   public:
    CgiProcess();
    ~CgiProcess();

    void cleanup();

    // execPath: path to executable (interpreter or script)
    // args: argv vector (argv[0] should be execPath)
    // env: environment entries as "KEY=VALUE"
    bool startProcess(const std::string&              execPath,
                      const std::vector<std::string>& args,
                      const std::vector<std::string>& env,
                      const std::string&              workingDir = "");

    /* Non-blocking I/O helpers:
	* - append to outputBuffer
	* - consume from inputBuffer
	*/
    // Returns true when stdin fully flushed and closed.
    bool flushInputOnce();
    // Returns true when EOF reached on stdout.
    bool readOutputOnce();

    bool hasExited(int* status);
    bool isValid() const;

    void setInputBuffer(const std::string& inputBuffer);
    void setOutputBuffer(const std::string& outputBuffer);

    int         getInputPipe() const;
    int         getOutputPipe() const;
    int         getChildPid() const;
    time_t      getStartTime() const;
    std::string getInputBuffer() const;
    std::string getOutputBuffer() const;

   private:
    int    childPid_;
    int    inputPipe_;
    int    outputPipe_;
    time_t startTime_;
    bool   isValid_;

    // good enough for now but will not scale well for large cgi output or input
    std::string outputBuffer_;
    std::string inputBuffer_;
};

#endif
