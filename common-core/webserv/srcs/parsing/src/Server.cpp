#include "../include/ConfigGett.hpp"

Server::Server(const std::map<std::string, std::vector<std::string> >& prmtrs, const std::string& name)
    : name(name), prmtrs(prmtrs)
{
	return ;
}

const std::string& Server::getName() const
{
    return name;
}

size_t Server::getHost() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it
        = prmtrs.find("host");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<size_t>(std::atoi(it->second[0].c_str()));
    return 0;
}

size_t Server::getClientMaxBodySize() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("client_max_body_size");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<size_t>(std::atoi(it->second[0].c_str()));
    return 0;
}

bool Server::getAutoIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("autoindex");
    if (it != prmtrs.end() && !it->second.empty())
	{
        const std::string& v = it->second[0];
        return (v == "on");
    }
    return false;
}

const std::string* Server::getRoot() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("root");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

const std::string* Server::getIndex() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("index");
    if (it != prmtrs.end() && !it->second.empty())
        return &(it->second[0]);
    return NULL;
}

int	 Server::getPort() const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.find("listen");
    if (it != prmtrs.end() && !it->second.empty())
        return static_cast<int>(std::atoi(it->second[0].c_str()));
    return 0;
}

const Location& Server::getLocation(const std::string& uri) const 
{
    std::map<std::string, Location>::const_iterator it = route.find(uri);
    if (it != route.end())
        return (it->second);
	else
		throw NotFoundUri();
	//TODO:magari girare un eccezione
}

const Location& Server::getCgiBin( void ) const 
{
    std::map<std::string, Location>::const_iterator it = route.find("cgi-bin");
    if (it != route.end())
        return (it->second);
	else
		throw NotFoundUri();
	//TODO:magari girare un eccezione
}

const std::string* Server::getErrorPageLocation(const std::string& uri, const std::string& nbrError) const
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

const std::string* Server::getErrorPage(const std::string& nbrError) const
{
    size_t pos;
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = prmtrs.begin(); it != prmtrs.end(); ++it)
    {
        if ((pos = it->first.find(nbrError, 0)) != std::string::npos)
            return &(it->second[0]);
    }
    return NULL;
}
const char* Server::NotFoundUri::what() const throw() {
    return "URI not found";
}
