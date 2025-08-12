/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload_stream.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:17:30 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:17:30 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "upload_stream.hpp"

UploadFileStream::UploadFileStream() : file_() {}

bool UploadFileStream::open(const std::string& path, bool append) { return file_.openForWrite(path, append); }

void UploadFileStream::close() { file_.close(); }

bool UploadFileStream::wantsFileWrite(const StreamBuffer& inBuf) const { return !file_.hasError() && (inBuf.size() > 0); }

int UploadFileStream::fileFd() const { return file_.fd(); }

ssize_t UploadFileStream::onFileWritable(StreamBuffer& inBuf) {
    if (inBuf.size() == 0) return 0;
    ssize_t n = file_.writeOnce(inBuf.data(), inBuf.size());
    if (n > 0) inBuf.consume(static_cast<size_t>(n));
    return n;
}


