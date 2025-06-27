/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 23:33:20 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/28 23:33:21 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/socket_guard.hpp"

namespace lib {
void SocketGuard::release() {
    released_ = true;
}
}  // namespace lib
