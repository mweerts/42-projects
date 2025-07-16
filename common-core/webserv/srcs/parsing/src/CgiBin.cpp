#include "../include/GlobalConfig.hpp"

CgiBin::CgiBin() {
    return;
}
CgiBin::CgiBin(
    const std::map<std::string, std::vector<std::string> >& passprmtrs,
    const std::string&                                      Addname)
    : name(Addname), prmtrs(passprmtrs) {
    return;
}

const std::string* CgiBin::getRoot() const {
    static const std::string defaultRoot = "./";
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("root");
    if (it != prmtrs.end() && !it->second.empty()) {
        return &(it->second[0]);
    }
    return &defaultRoot;
}

const std::vector<std::string>* CgiBin::getPath(void) const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("cgi_path");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second);
    return NULL;
}
const std::vector<std::string>* CgiBin::getExt(void) const {
    std::map<std::string, std::vector<std::string> >::const_iterator it =
        prmtrs.find("cgi_ext");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second);
    return NULL;
}