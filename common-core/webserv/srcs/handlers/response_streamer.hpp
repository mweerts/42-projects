/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_streamer.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 01:39:24 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/14 01:39:25 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_STREAMER_HPP
#define RESPONSE_STREAMER_HPP

#include <string>

#include "../handlers/file_streaming.hpp"

class HttpResponse;

class ResponseStreamer {
    enum State { HEADERS, BODY, COMPLETE };
	// Response type regular is a pre-built response (200, 404, etc.)
	enum ResponseType { REGULAR, FILE, CGI };

   public:
    ResponseStreamer();
    ~ResponseStreamer();

    void prepareResponse(HttpResponse& response);
    void prepareStaticFile(HttpResponse& response, const std::string& path);
    void prepareCgiResponse(HttpResponse& response);

    bool    isWritable() const;
    bool    isComplete() const;
    ssize_t writeNextChunk(int fd);

    State getState() const;
    bool  isStreamingFile() const;
    bool  isStreamingCgi() const;

    FileReadStream& getFileStream() const;
    bool            wantsFileRead() const;

    void reset();

   private:	
    State state_;
    ResponseType response_type_;

    std::string header_buffer_;
    size_t      header_sent_;

    std::string body_content_;
    size_t      body_sent_;
	
    FileReadStream read_stream_;

    ssize_t writeHeaders(int socket_fd);
    ssize_t writeBody(int socket_fd);
    ssize_t writeStaticFileBody(int socket_fd);
    ssize_t writeResponseBody(int socket_fd);
};

#endif
