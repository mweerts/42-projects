/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload_stream.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:17:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:17:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_STREAM_HPP
#define UPLOAD_STREAM_HPP

#include <string>

#include "lib/file_io.hpp"
#include "lib/stream_buffer.hpp"

// Glue object to stream an upload from client to a file using one-op-per-event.
class UploadFileStream {
   public:
    UploadFileStream();

    bool open(const std::string& path, bool append);
    void close();

    // Poll integration
    bool wantsFileWrite(const StreamBuffer& inBuf) const;  // POLLOUT when data present
    int  fileFd() const;

    // Returns: bytes written, 0 if nothing written, -1 on would-block/error
    ssize_t onFileWritable(StreamBuffer& inBuf);

   private:
    FileWriter file_;
};

#endif


