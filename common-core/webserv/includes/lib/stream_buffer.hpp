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

// Simple single-producer/single-consumer byte buffer with watermarks.
// Designed to make one-op-per-event I/O straightforward.
class StreamBuffer {
   public:
    StreamBuffer(size_t highWatermark, size_t lowWatermark)
        : buffer_(), high_(highWatermark), low_(lowWatermark) {}

    // Producer side
    bool wantProducer() const {
        return buffer_.size() < high_;
    }
    void append(const char* data, size_t length) {
        buffer_.append(data, length);
    }

    // Consumer side
    bool wantConsumer() const {
        return !buffer_.empty();
    }
    size_t size() const {
        return buffer_.size();
    }
    bool empty() const {
        return buffer_.empty();
    }

    // Expose direct read-only access for sending
    const char* data() const {
        return buffer_.c_str();
    }

    // Consume n bytes from the front
    void consume(size_t n) {
        (void)low_;
        if (n >= buffer_.size()) {
            buffer_.clear();
        } else {
            buffer_.erase(0, n);
        }
    }

   private:
    std::string buffer_;
    size_t      high_;
    size_t      low_;
};

#endif
