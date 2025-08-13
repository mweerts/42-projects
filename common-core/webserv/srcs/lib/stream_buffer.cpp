/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:43:38 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/13 20:43:39 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/stream_buffer.hpp"

#include "Logger.hpp"

StreamBuffer::StreamBuffer(size_t highWatermark, size_t lowWatermark)
    : buffer_(), high_(highWatermark), low_(lowWatermark) {
    if (low_ > high_)
        low_ = high_ / 2;  // protect against invalid watermarks
}

StreamBuffer::~StreamBuffer() {}

// if buffer is ready to receive more data.
bool StreamBuffer::wantProducer() const {
    return buffer_.size() < low_ && !isFull();
}

void StreamBuffer::append(const char* data, size_t length) {
    if (!data || length == 0)
        return;
    try {
        buffer_.append(data, length);
    } catch (const std::exception& e) {
        Logger::warning() << "Failed to append data to stream buffer: "
                          << e.what();
    }
}

bool StreamBuffer::isFull() const {
    return buffer_.size() >= high_;
}

// if buffer has data to be consumed.
bool StreamBuffer::wantConsumer() const {
    return !buffer_.empty();
}

// Consumes n bytes from the buffer.
void StreamBuffer::consume(size_t n) {
    (void)low_;
    if (n >= buffer_.size()) {
        buffer_.clear();
    } else {
        buffer_.erase(0, n);
    }
}

size_t StreamBuffer::size() const {
    return buffer_.size();
}

bool StreamBuffer::empty() const {
    return buffer_.empty();
}

const char* StreamBuffer::data() const {
    return buffer_.c_str();
}
