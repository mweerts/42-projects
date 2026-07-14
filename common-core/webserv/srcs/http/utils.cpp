#include "utils.hpp"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Logger.hpp"
#include "lib/file_utils.hpp"
#include "lib/utils.hpp"

std::string humanReadableSize(off_t size) {
    const char* suffixes[] = {"", "K", "M", "G", "T"};
    int         i = 0;
    double      dblSize = size;
    char        buf[10];

    if (dblSize < 10000) {
#ifdef __linux__
        snprintf(buf, sizeof(buf), "%ld", size);
#else
        snprintf(buf, sizeof(buf), "%lld", size);
#endif

        return std::string(buf);
    }

    while (dblSize >= 1024 && i < 4) {
        dblSize /= 1024;
        ++i;
    }
    if (i == 0)
#ifdef __linux__
        snprintf(buf, sizeof(buf), "%ld", size);
#else
        snprintf(buf, sizeof(buf), "%lld", size);
#endif
    else
        snprintf(buf, sizeof(buf), "%.0f%s", dblSize, suffixes[i]);

    return std::string(buf);
}

std::string trimSlashes(const std::string& path) {
    std::string trimmed = path;
    while (trimmed.length() > 1 && trimmed[trimmed.length() - 1] == '/') {
        trimmed.erase(trimmed.length() - 1);
    }
    return trimmed;
}

std::string getHtmlIndexPage(const std::string& root, const std::string& uri) {
    std::ostringstream       oss;
    std::string              path = trimSlashes(root) + trimSlashes(uri);
    std::vector<std::string> files;
    std::vector<std::string> dirs;

    oss << "<html><head><title>Index of " << uri << "</title></head>"
        << "<body><h1>Index of " << uri << "</h1><hr><pre>";

    // List files in the directory
    DIR* dir = opendir(path.c_str());
    if (dir) {
        struct dirent* entry;
        oss << "<a href=\"../\">../</a>\n";
        while ((entry = readdir(dir)) != NULL) {
            std::string name(entry->d_name);
            if (name == "." || name == "..")
                continue;
            std::string fullPath = trimSlashes(path) + "/" + trimSlashes(name);
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
                dirs.push_back(name);
            else
                files.push_back(name);
        }
        closedir(dir);
    }
    // Sort directories and files
    std::sort(dirs.begin(), dirs.end());
    std::sort(files.begin(), files.end());

    // Add directories to the output
    for (size_t i = 0; i < dirs.size(); ++i) {
        std::string name = dirs[i];
        std::string croppedName = name;
        std::string fullPath = trimSlashes(path) + "/" + trimSlashes(name);

        if (croppedName.length() > 50) {
            croppedName = croppedName.substr(0, 47);
            croppedName.append("..>");
        }
        oss << "<a href=\"" << trimSlashes(uri) << "/" << trimSlashes(name)
            << "/\">" << croppedName;
        if (croppedName.length() < 50)
            oss << "/";
        oss << "</a>";
        if (croppedName.length() < 50)
            oss << std::string(50 - croppedName.length(), ' ');
        else
            oss << std::string(51 - croppedName.length(), ' ');
        oss << lib::getLastModifiedTime(fullPath) << std::setw(8) << "-"
            << "\n";
    }
    // Add files to the output
    for (size_t i = 0; i < files.size(); ++i) {
        std::string name = files[i];
        std::string croppedName = name;
        std::string fullPath = trimSlashes(path) + "/" + trimSlashes(name);

        if (croppedName.length() > 50) {
            croppedName = croppedName.substr(0, 47);
            croppedName.append("..>");
        }

        oss << "<a href=\"" << trimSlashes(uri) << "/" << trimSlashes(name)
            << "\">" << croppedName << "</a>"
            << std::string(51 - croppedName.length(), ' ')
            << lib::getLastModifiedTime(fullPath) << std::setw(8)
            << humanReadableSize(lib::getFileSize(fullPath)) << "\n";
    }
    oss << "</pre><hr></body></html>";
    return oss.str();
}
