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

off_t getFileSize(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        return fileInfo.st_size;
    }
    return -1;
}

bool isPhpFile(const std::string& path) {
    return path.substr(path.find_last_of('.')) == ".php";
}

std::string GetHtmlErrorPage(HttpResponse& response) {
    std::ostringstream oss;
    oss << response.getStatusCode();
    return "<html><head><title>" + oss.str() + " " +
           GetHttpStatusText(response.getStatusCode()) +
           "</title></head><body><center><h1>" + oss.str() + " " +
           GetHttpStatusText(response.getStatusCode()) +
           "</h1></center><hr><center>" + response.getServerName() +
           "</center></body></html>";
}

static std::string getLastModifiedTime(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        char       buffer[80];
        struct tm* tm_info = localtime(&fileInfo.st_mtime);
        strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", tm_info);
        return std::string(buffer);
    }
    return "";
}

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

std::string getHtmlIndexPage(const std::string& root, const std::string& uri) {
    std::ostringstream oss;
    std::string        path = root + uri;

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
            std::string fullPath = path + "/" + name;
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
                oss << "<a href=\"" << name << "/\">" << name << "/</a>"
                    << std::string(50 - name.length(), ' ')
                    << getLastModifiedTime(fullPath) << std::setw(7) << "-"
                    << "\n";

            } else {
                oss << "<a href=\"" << name << "\">" << name << "</a>"
                    << std::string(51 - name.length(), ' ')
                    << getLastModifiedTime(fullPath) << std::setw(7)
                    << humanReadableSize(getFileSize(fullPath)) << "\n";
            }
        }
        closedir(dir);
    }
    return oss.str();
}
