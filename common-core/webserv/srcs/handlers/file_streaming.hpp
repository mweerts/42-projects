/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_stream.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:15:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:15:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_STREAMING_HPP
#define FILE_STREAMING_HPP

#include <string>
#include "lib/file_io.hpp"
#include "lib/stream_buffer.hpp"

/*
 * Streams a static file to a client socket, reading from the file into a buffer
 * and allowing the server to send buffered data one chunk per event.
 */
class FileReadStream {
   public:
    FileReadStream();
    ~FileReadStream();

    bool open(const std::string& path);
    void close();

    bool wantsFileRead() const;
    int  fileFd() const;

    ssize_t onFileReadable();
    StreamBuffer& outBuffer();
	
    bool isEof() const;

   private:
    FileReader   file_;
    StreamBuffer outBuf_;
    char         readBuffer_[4096];
};

/*
 * Streams a file to a client socket, writing from a buffer to the file.
 */
class FileWriteStream {
   public:
    FileWriteStream();
    ~FileWriteStream();

    bool open(const std::string& path, bool append);
    void close();

    bool wantsFileWrite(const StreamBuffer& inBuf) const;
    int  fileFd() const;

    ssize_t onFileWritable(StreamBuffer& inBuf);

   private:
    FileWriter file_;
};

#endif
