/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:24:28 by llebugle          #+#    #+#             */
/*   Updated: 2025/06/25 18:24:29 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/Logger.hpp"

int main() {
    Logger::setLevel(LOG_LEVEL_DEBUG);
    Logger::enableColors(true);
    
    Logger::info() << "... initializing server ...";
    return 0;
}