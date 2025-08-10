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
    CgiProcess()
        : childPid_(-1),
          inputPipe_(-1),
          outputPipe_(-1),
          startTime_(time(NULL)),
          isValid_(false) {}

    ~CgiProcess() {
        cleanup();
    }

    void cleanup();

    // Start the CGI child process.
    // execPath: path to executable (interpreter or script)
    // args: argv vector (argv[0] should be execPath)
    // env: environment entries as "KEY=VALUE"
    bool startProcess(const std::string&              execPath,
                      const std::vector<std::string>& args,
                      const std::vector<std::string>& env,
                      const std::string&              workingDir = "");

    // Non-blocking I/O helpers (append to outputBuffer, consume from
    // inputBuffer) Returns true when stdin fully flushed and closed.
    bool flushInputOnce();
    // Returns true when EOF reached on stdout.
    bool readOutputOnce();

    // Child state
    bool hasExited(int* status);
	bool isValid() const;

	// Setters
	void setInputBuffer(const std::string& inputBuffer);
	void setOutputBuffer(const std::string& outputBuffer);

	// Getters
    int  getInputPipe() const;
    int  getOutputPipe() const;
	int  getChildPid() const;
	time_t getStartTime() const;
	std::string getInputBuffer() const;
	std::string getOutputBuffer() const;

   private:

    int         childPid_;
    int         inputPipe_;
    int         outputPipe_;
    time_t      startTime_;
    bool        isValid_;

	// should this be in this class?
    std::string outputBuffer_;  // Accumulate CGI output
    std::string inputBuffer_;   // Data to send to CGI
};

#endif
