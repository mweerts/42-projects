#ifndef UTILS_HPP
#define UTILS_HPP

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <cstdio>

#include "HttpResponse.hpp"

bool pathExist(const std::string& path);
bool isFile(const std::string& path);
bool isDirectory(const std::string& path);
bool isReadable(const std::string& path);
bool isPhpFile(const std::string& path);

off_t getFileSize(const std::string& path);

std::string GetHtmlErrorPage(HttpResponse& response);
std::string getHtmlIndexPage(const std::string& root, const std::string& uri);

std::string humanReadableSize(off_t size);

#endif