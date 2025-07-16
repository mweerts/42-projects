#include "../include/ConfigGett.hpp"

ServerConf::ServerConf(const std::map<std::string, std::vector<std::string> >& prmtrs, const std::string& name)
    : name(name), prmtrs(prmtrs)
{
	return ;
}

const std::string& ServerConf::getName() const
{
    return name;
}

const std::string ServerConf::getHost() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it
        = prmtrs.find("host");
    if (it != prmtrs.end() && !it->second.empty())
        // return static_cast<size_t>(std::atoi(it->second[0].c_str()));
        return it->second[0];
    return "";
}

size_t ServerConf::getClientMaxBodySize() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("client_max_body_size");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<size_t>(std::atoi(it->second[0].c_str()));
    return 0;
}

bool ServerConf::getAutoIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("autoindex");
    if (it != prmtrs.end() && !it->second.empty())
	{
        const std::string& v = it->second[0];
        return (v == "on");
    }
    return false;
}

const std::string* ServerConf::getRoot() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("root");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

const std::string* ServerConf::getIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("index");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

int	 ServerConf::getPort() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("listen");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<int>(std::atoi(it->second[0].c_str()));
    return 0;
}

const Location& ServerConf::getLocation(const std::string& uri) const 
{
    std::map<std::string, Location>::const_iterator it = route.find(uri);
    if (it != route.end())
        return (it->second);
	else
		throw NotFoundUri();
	//TODO:magari girare un eccezione
}

const Location& ServerConf::getCgiBin( void ) const 
{
    std::map<std::string, Location>::const_iterator it = route.find("cgi-bin");
    if (it != route.end())
        return (it->second);
	else
		throw NotFoundUri();
	//TODO:magari girare un eccezione
}

const std::string* ServerConf::getErrorPageLocation(const std::string& uri, const std::string& nbrError) const
{
    size_t pos;
    std::map<std::string, Location>::const_iterator itLoc = route.find(uri);
    if (itLoc != route.end())
    {
        const std::map<std::string, std::vector<std::string> >& locPrm = itLoc->second.prmtrs;
        for (std::map<std::string, std::vector<std::string> >::const_iterator it = locPrm.begin(); it != locPrm.end(); ++it)
        {
            if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
                return &(it->second[0]);
        }
    }
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.begin(); it != prmtrs.end(); ++it)
    {
        if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
            return &(it->second[0]);
    }
    return NULL;
}

const std::string* ServerConf::getErrorPage(const std::string& nbrError) const
{
    size_t pos;
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.begin(); it != prmtrs.end(); ++it)
    {
        if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
            return &(it->second[0]);
    }
    return NULL;
}
const char* ServerConf::NotFoundUri::what() const throw() {
    return "URI not found";
}
