#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/stat.h>

#include <string>

#include "HttpResponse.hpp"

std::string GetHtmlErrorPage(HttpResponse& response, const std::string& error_message = "");
std::string getHtmlIndexPage(const std::string& root, const std::string& uri);

std::string humanReadableSize(off_t size);

#endif
