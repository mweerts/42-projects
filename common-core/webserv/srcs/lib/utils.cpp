/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:52:37 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/10 18:52:40 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace lib {
std::string extractQueryFromUri(const std::string& uri) {
    size_t qpos = uri.find('?');
    return (qpos == std::string::npos) ? "" : uri.substr(qpos + 1);
}

std::string extractPathFromUri(const std::string& uri) {
    size_t qpos = uri.find('?');
    return (qpos == std::string::npos) ? "" : uri.substr(0, qpos);
}
}  // namespace lib
