/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_guard.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:08:39 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/18 16:08:40 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/fd_guard.hpp"

FdGuard::FdGuard(int fd) : fd_(fd), released_(false) {}

FdGuard::~FdGuard() {
    if (fd_ >= 0 && !released_) {
        close(fd_);
		fd_ = -1;
    }
}

int FdGuard::release() {
    released_ = true;
	int tmp = fd_;
	fd_ = -1;
    return tmp;
}

int FdGuard::get() const {
    return fd_;
}
