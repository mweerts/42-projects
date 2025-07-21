/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <llebugle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:00:00 by llebugle          #+#    #+#             */
/*   Updated: 2025/01/27 10:00:00 by llebugle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/utils.hpp"

#include <sys/stat.h>
#include <unistd.h>

namespace lib {

bool pathExist(const std::string& path) {
    struct stat fileInfo;
    return stat(path.c_str(), &fileInfo) == 0;
}

bool isFile(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISREG(fileInfo.st_mode);
}

bool isDirectory(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_ISDIR(fileInfo.st_mode);
}

bool isReadable(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IRUSR & fileInfo.st_mode && !access(path.c_str(), R_OK);
}

bool isWritable(const std::string& path) {
    struct stat fileInfo;
    stat(path.c_str(), &fileInfo);
    return S_IWUSR & fileInfo.st_mode && !access(path.c_str(), W_OK);
}

off_t getFileSize(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        return fileInfo.st_size;
    }
    return -1;
}

}  // namespace lib 
