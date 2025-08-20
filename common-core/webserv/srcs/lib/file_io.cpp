/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+ */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:12:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:12:30 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/file_io.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

FileReader::FileReader() : fd_(-1), eof_(false), err_(false) {}
FileReader::~FileReader() {
    close();
}

bool FileReader::openForRead(const std::string& path) {
    close();
    fd_ = ::open(path.c_str(), O_RDONLY
// #ifdef __APPLE__
//                                    | O_NONBLOCK
// #endif
    );
    eof_ = false;
    err_ = (fd_ < 0);
    return fd_ >= 0;
}

void FileReader::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

int FileReader::fd() const {
    return fd_;
}
bool FileReader::eof() const {
    return eof_;
}
bool FileReader::hasError() const {
    return err_;
}

ssize_t FileReader::readOnce(char* buf, size_t maxLen) {
    if (fd_ < 0 || err_ || eof_ || buf == NULL || maxLen == 0)
        return -1;
    ssize_t n = ::read(fd_, buf, maxLen);
    if (n > 0)
        return n;
    if (n == 0) {
        eof_ = true;
        return 0;
    }
	if (n < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -2; // Would block, try again later
        }
        err_ = true;
        return -1;
    }
    return -1;
}

FileWriter::FileWriter() : fd_(-1), err_(false) {}
FileWriter::~FileWriter() {
    close();
}

bool FileWriter::openForWrite(const std::string& path, bool append) {
    close();
    int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
#ifdef __APPLE__
    flags |= O_NONBLOCK;
#endif
    fd_ = ::open(path.c_str(), flags, 0644);
    err_ = (fd_ < 0);
    return fd_ >= 0;
}

void FileWriter::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

int FileWriter::fd() const {
    return fd_;
}

bool FileWriter::hasError() const {
    return err_;
}

ssize_t FileWriter::writeOnce(const char* buf, size_t len) {
    if (fd_ < 0 || err_ || !buf)
        return -1;
    if (len == 0)
        return 0;
    ssize_t n = ::write(fd_, buf, len);
    if (n >= 0)
        return n;
    return -1;
}
