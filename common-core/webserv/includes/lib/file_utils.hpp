#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>

namespace lib {
// File system utility functions
bool  pathExist(const std::string& path);
bool  isFile(const std::string& path);
bool  isDirectory(const std::string& path);
bool  isReadable(const std::string& path);
bool  isWritable(const std::string& path);
bool  isExecutable(const std::string& path);
long  getFileSize(const std::string& path);

}  // namespace lib

#endif
