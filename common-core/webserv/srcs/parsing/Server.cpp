#include "GlobalConfig.hpp"

// Initialize static members
CgiBin ServerConfig::globalCgiBin_;
bool ServerConfig::globalCgiInitialized_ = false;

ServerConfig::ServerConfig(
    const std::map<std::string, std::vector<std::string> >& prmtrs,
    const std::string&                                      name)
    : name(name), prmtrs(prmtrs) {
    return;
}

const std::string ServerConfig::getUploadDir( void ) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("upload_dir");
    if (it != prmtrs.end() && !it->second.empty())
	{
        return (it->second[0]);
	}
	return "";
}

const std::string ServerConfig::getTmpFolder( void ) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("tmp_folder");
    if (it != prmtrs.end() && !it->second.empty())
	{
        return (it->second[0]);
	}
	return "";
}

const std::string ServerConfig::getServerName( void ) const
{
	static const std::string DefaulServerName = "webserver";
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("server_name");
    if (it != prmtrs.end() && !it->second.empty())
	{
        return (it->second[0]);
	}
	return DefaulServerName;
}

const std::string& ServerConfig::getName() const {
    return name;
}

const std::string ServerConfig::getHost() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("host");
    if (it != prmtrs.end() && !it->second.empty())
        // return static_cast<size_t>(std::atoi(it->second[0].c_str()));
        return it->second[0];
    return "127.0.0.1";
}

size_t ServerConfig::getClientMaxBodySize() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("client_max_body_size");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<size_t>(std::atoi(it->second[0].c_str()));
    return 1048576; // 1mb
}

bool ServerConfig::getAutoIndex() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("autoindex");
    if (it != prmtrs.end() && !it->second.empty()) {
        const std::string& v = it->second[0];
        return (v == "on");
    }
    return false;
}

const std::string ServerConfig::getRoot() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("root");
    if (it != prmtrs.end() && !it->second.empty())
        return (it->second[0]);
    return "./";
}

const std::string* ServerConfig::getIndex() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("index");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

int ServerConfig::getPort() const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("listen");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<int>(std::atoi(it->second[0].c_str()));
    return 0;
}

const Location* ServerConfig::getLocation(const std::string& uri) const {
    const Location* bestMatch = NULL;
    size_t bestLength = 0;

    std::map<std::string, Location>::const_iterator itFind = route.find(uri);
    if (itFind != route.end()){
        return &(itFind->second);
	}
    for (std::map<std::string, Location>::const_iterator it = route.begin(); it != route.end(); ++it) {
        const std::string& path = it->first;
        if (uri.compare(0, path.length(), path) == 0) {
            if (uri.length() == path.length() || uri[path.length()] == '/') {
                if (path.length() > bestLength) {
                    bestMatch = &(it->second);
                    bestLength = path.length();
                }
            }
        }
    }
    return bestMatch;
}


const std::string* ServerConfig::getErrorPageLocation(
    const std::string& uri, const std::string& nbrError) const {
    size_t                                          pos;
    std::map<std::string, Location>::const_iterator itLoc = route.find(uri);
    if (itLoc != route.end()) {
        const std::map<std::string, std::vector<std::string> >& locPrm =
            itLoc->second.prmtrs;
        for (std::map<std::string, std::vector<std::string> >::const_iterator
                 it = locPrm.begin();
             it != locPrm.end(); ++it) {
            if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
                return &(it->second[0]);
        }
    }
    for (std::map<std::string, std::vector<std::string> >::const_iterator it =
             prmtrs.begin();
         it != prmtrs.end(); ++it) {
        if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
            return &(it->second[0]);
    }
	static std::string empty = "";
    return &empty;
}

const std::string* ServerConfig::getErrorPage(
    const std::string& nbrError) const {
    size_t pos;
	static std::string def = "";
    for (std::map<std::string, std::vector<std::string> >::const_iterator it =
             prmtrs.begin();
         it != prmtrs.end(); ++it) {
        if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
            return &(it->second[0]);
    }
	return NULL;
}

const char* ServerConfig::NotFoundUri::what() const throw() {
    return "URI not found";
}

void ServerConfig::setCgi(const CgiBin& add) {
    if (globalCgiInitialized_)
        return;
    else {
        globalCgiBin_ = add;
        globalCgiInitialized_ = true;
    }
    this->Cgi = true;
}
const CgiBin& ServerConfig::getCgiBin(void) const {
    return globalCgiBin_;
}
