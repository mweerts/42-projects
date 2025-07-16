
#include "../include/GlobalConfig.hpp"

Location::Location(
    const std::map<std::string, std::vector<std::string> >& passprmtrs,
    const std::string&                                      nameLocation)
    : name(nameLocation), prmtrs(passprmtrs) {
    return;
}

const std::string& Location::getName(void) {
    return this->name;
}

bool	Location::getMethodIsAllowed(const std::string& method) const
{

	std::map<std::string, std::vector<std::string> >::const_iterator it = this->prmtrs.find("allow_methods");
	if (it != this->prmtrs.end())
	{
		for (size_t i = 0; i < it->second.size(); ++i)
		{
			if (method == it->second[i])
				return true;
		}
	}
	return false;
}


size_t Location::getClientMaxBodySize() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("client_max_body_size");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<size_t>(std::atoi(it->second[0].c_str()));
    return 0;
}

bool Location::getAutoIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("autoindex");
    if (it != prmtrs.end() && !it->second.empty())
	{
        const std::string& v = it->second[0];
        return (v == "on");
    }
    return false;
}

const std::string* Location::getReturn() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("return");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

const std::string* Location::getIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("index");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

const std::string* Location::getAlias() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("alias");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

const std::string* Location::getRoot() const
{
	static const std::string defaultRoot = "./";
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("root");
    if (it != prmtrs.end() && !it->second.empty())
	{
        return &(it->second[0]);
	}
	return &defaultRoot;
}