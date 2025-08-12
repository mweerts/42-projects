/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_stream.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:15:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:15:30 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "static_stream.hpp"

StaticFileStream::StaticFileStream() : file_(), outBuf_(65536, 16384) {}

bool StaticFileStream::open(const std::string& path) { return file_.openForRead(path); }

void StaticFileStream::close() { file_.close(); }

bool StaticFileStream::wantsFileRead() const { return !file_.eof() && !file_.hasError() && outBuf_.wantProducer(); }

int StaticFileStream::fileFd() const { return file_.fd(); }

ssize_t StaticFileStream::onFileReadable() {
    ssize_t n = file_.readOnce(scratch_, sizeof(scratch_));
    if (n > 0) outBuf_.append(scratch_, static_cast<size_t>(n));
    return n;
}

StreamBuffer& StaticFileStream::outBuffer() { return outBuf_; }

bool StaticFileStream::isEof() const { return file_.eof(); }


