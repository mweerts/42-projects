#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/stat.h>
#include <unistd.h>

#include <string>

bool pathExist(const std::string& path);
bool isFile(const std::string& path);
bool isDirectory(const std::string& path);
bool isReadable(const std::string& path);
bool isPhpFile(const std::string& path);

#endif