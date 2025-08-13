/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_stream.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+ */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:15:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:15:30 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_streaming.hpp"

FileReadStream::FileReadStream() : file_(), outBuf_() {}

FileReadStream::~FileReadStream() {
    close();
}

bool FileReadStream::open(const std::string& path) {
    return file_.openForRead(path);
}

void FileReadStream::close() {
    file_.close();
}

bool FileReadStream::wantsFileRead() const {
    return !file_.eof() && !file_.hasError() && outBuf_.wantProducer();
}

int FileReadStream::fileFd() const {
    return file_.fd();
}

// returns bytes read, 0 on EOF, -1 on no progress/error
ssize_t FileReadStream::onFileReadable() {
    ssize_t n = file_.readOnce(readBuffer_, sizeof(readBuffer_));
    if (n > 0)
        outBuf_.append(readBuffer_, static_cast<size_t>(n));
    return n;
}

StreamBuffer& FileReadStream::outBuffer() {
    return outBuf_;
}

bool FileReadStream::isEof() const {
    return file_.eof();
}

FileWriteStream::FileWriteStream() : file_() {}

FileWriteStream::~FileWriteStream() {
    close();
}

bool FileWriteStream::open(const std::string& path, bool append) {
    return file_.openForWrite(path, append);
}

void FileWriteStream::close() {
    file_.close();
}

bool FileWriteStream::wantsFileWrite(const StreamBuffer& inBuf) const {
    return !file_.hasError() && (inBuf.size() > 0);
}

int FileWriteStream::fileFd() const {
    return file_.fd();
}

// returns bytes written, 0 if nothing written, -1 on would-block/error
ssize_t FileWriteStream::onFileWritable(StreamBuffer& inBuf) {
    if (inBuf.size() == 0)
        return 0;
    ssize_t n = file_.writeOnce(inBuf.data(), inBuf.size());
    if (n > 0)
        inBuf.consume(static_cast<size_t>(n));
    return n;
}
