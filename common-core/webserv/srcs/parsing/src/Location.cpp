
#include "../include/GlobalConfig.hpp"

Location::Location(const std::map<std::string, std::vector<std::string> >& passprmtrs, const std::string& nameLocation)
    : name(nameLocation), prmtrs(passprmtrs)
{
	return ;
}

const std::string& Location::getName(void)
{
	return this->name;
}

