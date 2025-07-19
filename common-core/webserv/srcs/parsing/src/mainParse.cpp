#include "../../../includes/Logger.hpp"
#include "../include/ConfigProcessor.hpp"
#include "../include/GlobalConfig.hpp"
int	main(int argv, char **argc)
{
	if (argv == 2)
	{
	    Logger::setLevel(LOG_LEVEL_DEBUG);
	    Logger::enableColors(true);
		ConfigProcessor conf((std::string(argc[1])));
		if (conf.tokenize() == 1)
			return (1);
		conf.printAllTree();
		GlobalConfig getter(conf);
		const std::vector<ServerConfig> ClassServers = getter.getServers();
		for (size_t i = 0; i < ClassServers.size(); ++i)
		{
			Logger::info() << ClassServers[i].getClientMaxBodySize();
			Logger::info() << ClassServers[i].getHost();
			Logger::info() << ClassServers[i].getName();
			Logger::info() << ClassServers[i].getPort();
			Logger::info() << ClassServers[i].getAutoIndex();
			Logger::info() << ClassServers[i].getRoot();
			Logger::info() << ClassServers[i].getUplaodDir();
			std::cout << "\n\n";

			const Location *uri = (ClassServers[i].getLocation("NULLA"));
			if (uri != NULL)
			{
				std::cout << uri->getName() << std::endl;
				std::cout << uri->getMethodIsAllowed("DELETE") << std::endl;
			}
			else
			{
				Logger::error() << "Location non esiste";
			}
		}

	}

    return (0);
}
