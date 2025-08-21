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

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "Logger.hpp"
#include "lib/fd_guard.hpp"

CgiProcess::CgiProcess()
    : childPid_(-1),
      inputPipe_(-1),
      outputPipe_(-1),
      startTime_(time(NULL)),
      isValid_(false) {}

CgiProcess::~CgiProcess() {
    cleanup();
}

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

bool CgiProcess::startProcess(const std::string&              exec_path,
                              const std::vector<std::string>& args,
                              const std::vector<std::string>& env,
                              const std::string&              workingDir) {

	if (exec_path.empty() || args.empty())
		return false;

    int inPipe[2] = {-1, -1};
    int outPipe[2] = {-1, -1};

    if (pipe(inPipe) == -1)
        return false;

    if (pipe(outPipe) == -1) {
        close(inPipe[0]);
        close(inPipe[1]);
        return false;
    }

    FdGuard inReadGuard(inPipe[0]);
    FdGuard inWriteGuard(inPipe[1]);
    FdGuard outReadGuard(outPipe[0]);
    FdGuard outWriteGuard(outPipe[1]);

    fcntl(inWriteGuard.get(), F_SETFL, O_NONBLOCK);
    fcntl(outReadGuard.get(), F_SETFL, O_NONBLOCK);

    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }

    if (pid == 0) {
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

        std::vector<char*> argv;
        argv.reserve(args.size() + 1);
        for (size_t i = 0; i < args.size(); ++i)
            argv.push_back(const_cast<char*>(args[i].c_str()));
        argv.push_back(NULL);

        std::vector<char*> envp;
        envp.reserve(env.size() + 1);
        for (size_t i = 0; i < env.size(); ++i)
            envp.push_back(const_cast<char*>(env[i].c_str()));
        envp.push_back(NULL);

        execve(exec_path.c_str(), &argv[0], &envp[0]);
        _exit(1);
    }

    childPid_ = pid;
    inputPipe_ = inWriteGuard.release();
    outputPipe_ = outReadGuard.release();

    isValid_ = true;
    startTime_ = time(NULL);
    return true;
}

static void resetFd(int& fd) {
	if (fd != -1) {
		close(fd);
		fd = -1;
	}
}

bool CgiProcess::flushInputOnce() {
    if (inputPipe_ == -1)
        return true;
    if (inputBuffer_.empty()) {
        resetFd(inputPipe_);
        return true;
    }
    ssize_t w = write(inputPipe_, inputBuffer_.data(), inputBuffer_.size());
    if (w < 0) {
        resetFd(inputPipe_);
        Logger::error("Flush input once failed");
        return false;
    } else if (w == 0) {
        resetFd(inputPipe_);
        return true;
    }
    inputBuffer_.erase(0, static_cast<size_t>(w));
    if (inputBuffer_.empty()) {
        resetFd(inputPipe_);
        return true;
    }
    return false;
}

bool CgiProcess::readOutputOnce() {
    if (outputPipe_ == -1)
        return true;
    char    buf[4096];
    ssize_t r = read(outputPipe_, buf, sizeof(buf));
    if (r < 0) {
        Logger::error("Read output once failed");
        resetFd(outputPipe_);
        return true;
    } else if (r == 0) {
        resetFd(outputPipe_);
        return true;
    }
    outputBuffer_.append(buf, r);
    return false;
}

bool CgiProcess::hasExited(int* status) {
    int   st = 0;
    pid_t r = waitpid(childPid_, &st, WNOHANG);
    if (r == childPid_) {
        if (status)
            *status = st;
        return true;
    }
    return false;
}

/* ============ Public helpers ============ */

void CgiProcess::setInputBuffer(const std::string& inputBuffer) {
    inputBuffer_ = inputBuffer;
}

void CgiProcess::setOutputBuffer(const std::string& outputBuffer) {
    outputBuffer_ = outputBuffer;
}

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
