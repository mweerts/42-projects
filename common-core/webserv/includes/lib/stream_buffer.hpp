/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream_buffer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:10:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/12 16:10:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STREAM_BUFFER_HPP
#define STREAM_BUFFER_HPP

#include <sys/types.h>

#include <string>

/*
 * Simple single-producer/single-consumer byte buffer with watermarks.
 */
class StreamBuffer {
    static const size_t DEFAULT_LOW_WATERMARK = 16384;   // 16kb
    static const size_t DEFAULT_HIGH_WATERMARK = 65536;  // 64kb

   public:
    explicit StreamBuffer(size_t highWatermark = DEFAULT_HIGH_WATERMARK,
                 size_t lowWatermark = DEFAULT_LOW_WATERMARK);
    ~StreamBuffer();

    bool wantProducer() const;
    bool wantConsumer() const;
	
    void append(const char* data, size_t length);
    void consume(size_t n);

    size_t size() const;
    bool isFull() const;
    bool empty() const;

    const char* data() const;


   private:
    std::string buffer_;
    size_t      high_;
    size_t      low_;
};

#endif
