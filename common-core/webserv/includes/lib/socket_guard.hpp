/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 23:33:08 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/28 23:33:09 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_GUARD_HPP
#define SOCKET_GUARD_HPP

#include <unistd.h>

namespace lib {
class SocketGuard {
   public:
    explicit SocketGuard(int& fd) : fd_(fd), released_(false) {}
    ~SocketGuard() {
        if (fd_ >= 0 && !released_)
            close(fd_);
    }
    void release();

   private:
    int& fd_;
    bool released_;
};
}  // namespace lib

#endif
