/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_process.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:42:13 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/05 15:42:14 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi_process.hpp"
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <string>

void CgiProcess::cleanup() {
    if (inputPipe_ != -1) {
        close(inputPipe_);
        inputPipe_ = -1;
    }
    if (outputPipe_ != -1) {
        close(outputPipe_);
        outputPipe_ = -1;
    }
    if (childPid_ > 0) {
        kill(childPid_, SIGTERM);
        waitpid(childPid_, NULL, WNOHANG);
        childPid_ = -1;
    }
    isValid_ = false;
}

bool CgiProcess::startProcess(const std::string&              execPath,
                              const std::vector<std::string>& args,
                              const std::vector<std::string>& env,
                              const std::string&              workingDir) {
    int inPipe[2] = {-1, -1};
    int outPipe[2] = {-1, -1};

    if (pipe(inPipe) == -1) return false;
    if (pipe(outPipe) == -1) {
        close(inPipe[0]);
        close(inPipe[1]);
        return false;
    }

    // Non-blocking parent ends
    fcntl(inPipe[1], F_SETFL, O_NONBLOCK);
    fcntl(outPipe[0], F_SETFL, O_NONBLOCK);

    pid_t pid = fork();
    if (pid == -1) {
        close(inPipe[0]); close(inPipe[1]);
        close(outPipe[0]); close(outPipe[1]);
        return false;
    }

    if (pid == 0) {
        // Child
        // Reset flags for duped fds
        fcntl(inPipe[0], F_SETFL, 0);
        fcntl(outPipe[1], F_SETFL, 0);

        close(inPipe[1]);
        close(outPipe[0]);

        if (dup2(inPipe[0], STDIN_FILENO) == -1 ||
            dup2(outPipe[1], STDOUT_FILENO) == -1) {
            _exit(1);
        }
        close(inPipe[0]);
        close(outPipe[1]);

		if (!workingDir.empty()) {
            if (chdir(workingDir.c_str()) != 0) {
                _exit(1);
            }
        }

        // Build argv
        std::vector<char*> argv;
        argv.reserve(args.size() + 1);
        for (size_t i = 0; i < args.size(); ++i) {
            argv.push_back(const_cast<char*>(args[i].c_str()));
        }
        argv.push_back(NULL);

        // Build envp
        std::vector<char*> envp;
        envp.reserve(env.size() + 1);
        for (size_t i = 0; i < env.size(); ++i) {
            envp.push_back(const_cast<char*>(env[i].c_str()));
        }
        envp.push_back(NULL);

        execve(execPath.c_str(), &argv[0], &envp[0]);
        _exit(1);
    }

    // Parent
    childPid_ = pid;
    close(inPipe[0]);
    close(outPipe[1]);
    inputPipe_ = inPipe[1];
    outputPipe_ = outPipe[0];
    isValid_ = true;
    return true;
}

bool CgiProcess::flushInputOnce() {
    if (inputPipe_ == -1) return true; // already closed
    if (inputBuffer_.empty()) {
        close(inputPipe_);
        inputPipe_ = -1;
        return true;
    }
    ssize_t w = write(inputPipe_, inputBuffer_.data(), inputBuffer_.size());
    if (w > 0) {
        inputBuffer_.erase(0, static_cast<size_t>(w));
        if (inputBuffer_.empty()) {
            close(inputPipe_);
            inputPipe_ = -1;
            return true;
        }
        return false;
    }
    if (w < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        return false;
    }
    // On error, close to avoid spinning
    close(inputPipe_);
    inputPipe_ = -1;
    return true;
}

bool CgiProcess::readOutputOnce() {
    if (outputPipe_ == -1) return true;
    char buf[4096];
    ssize_t r = read(outputPipe_, buf, sizeof(buf));
    if (r > 0) {
        outputBuffer_.append(buf, r);
        return false;
    }
    if (r == 0) {
        close(outputPipe_);
        outputPipe_ = -1;
        return true;
    }
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
    }
    // Error: close and mark complete
    close(outputPipe_);
    outputPipe_ = -1;
    return true;
}

bool CgiProcess::hasExited(int* status) {
    int  st = 0;
    pid_t r = waitpid(childPid_, &st, WNOHANG);
    if (r == childPid_) {
        if (status) *status = st;
        return true;
    }
    return false;
}

/* ============ Setters ============ */

void CgiProcess::setInputBuffer(const std::string& inputBuffer) {
    inputBuffer_ = inputBuffer;
}

void CgiProcess::setOutputBuffer(const std::string& outputBuffer) {
    outputBuffer_ = outputBuffer;
}

/* ============ Public helpers ============ */

bool CgiProcess::isValid() const {
    return isValid_;
}

int CgiProcess::getInputPipe() const {
    return inputPipe_;
}

int CgiProcess::getOutputPipe() const {
    return outputPipe_;
}

int CgiProcess::getChildPid() const {
    return childPid_;
}

time_t CgiProcess::getStartTime() const {
    return startTime_;
}

std::string CgiProcess::getInputBuffer() const {
    return inputBuffer_;
}

std::string CgiProcess::getOutputBuffer() const {
    return outputBuffer_;
}
