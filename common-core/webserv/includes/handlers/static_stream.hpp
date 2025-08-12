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

#ifndef STATIC_STREAM_HPP
#define STATIC_STREAM_HPP

#include <string>

#include "lib/file_io.hpp"
#include "lib/stream_buffer.hpp"

// Glue object to stream a static file to a client socket using one-op-per-event.
class StaticFileStream {
   public:
    StaticFileStream();

    // Returns false on open failure (caller maps to HTTP error)
    bool open(const std::string& path);
    void close();

    // Poll integration
    bool wantsFileRead() const;  // poll for POLLIN on file
    int  fileFd() const;

    // I/O steps (one call per ready event)
    // Returns: bytes read, 0 on EOF, -1 on no progress/error
    ssize_t onFileReadable();

    // Buffer exposed to client writer
    StreamBuffer& outBuffer();
    bool          isEof() const;

   private:
    FileReader   file_;
    StreamBuffer outBuf_;
    char         scratch_[4096];
};

#endif


