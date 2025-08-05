/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_process.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:42:13 by llebugle          #+#    #+#             */
/*   Updated: 2025/08/05 15:42:14 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi_process.hpp"
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

void CgiProcess::cleanup() {
    if (inputPipe != -1) {
        close(inputPipe);
        inputPipe = -1;
    }
    if (outputPipe != -1) {
        close(outputPipe);
        outputPipe = -1;
    }
    if (childPid > 0) {
        kill(childPid, SIGTERM);
        waitpid(childPid, NULL, WNOHANG);
        childPid = -1;
    }
    isValid = false;
}

