/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:12:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:12:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <string>

// Lightweight wrappers for file streaming roles used by the event loop.
// Note: we do not read errno after read/write; caller uses poll revents for errors.

class FileReader {
   public:
    FileReader();
    ~FileReader();

    bool   openForRead(const std::string& path);
    void   close();
    int    fd() const;
    bool   eof() const;
    bool   hasError() const;
    // Read at most once into provided buffer (appends to it). Returns bytes read, 0 on EOF, -1 on would-block or error.
    ssize_t readOnce(char* buf, size_t maxLen);

   private:
    int  fd_;
    bool eof_;
    bool err_;
};

class FileWriter {
   public:
    FileWriter();
    ~FileWriter();

    bool   openForWrite(const std::string& path, bool append);
    void   close();
    int    fd() const;
    bool   hasError() const;
    // Write at most once from provided buffer. Returns bytes written, 0 if nothing written, -1 on would-block or error.
    ssize_t writeOnce(const char* buf, size_t len);

   private:
    int  fd_;
    bool err_;
};

#endif


