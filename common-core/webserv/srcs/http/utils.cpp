#include "utils.hpp"

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

bool isPhpFile(const std::string& path) {
    return path.substr(path.find_last_of('.')) == ".php";
}