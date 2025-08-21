/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_guard.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:58:49 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/18 15:58:51 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_GUARD_HPP
#define FD_GUARD_HPP

#include <unistd.h>

/*
 * Wraps a file descriptor and ensures it is closed when the object is destroyed.
 *
 * Usage:
 * FdGuard guard(fd);
 * // fd is closed when guard goes out of scope
 *
 * FdGuard guard(fd);
 * guard.release();
 * // transfer ownership of fd to the caller
 */
class FdGuard {
   public:
    FdGuard(int fd);
    ~FdGuard();

    int release();
	int get() const;

   private:
    int  fd_;
    bool released_;

    FdGuard(const FdGuard&);
    FdGuard& operator=(const FdGuard&);
};

#endif
