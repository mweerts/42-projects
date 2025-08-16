

#include "../srcs/parser/GlobalConfig.hpp"
#include "../srcs/parser/ConfigProcessor.hpp"
#include "../srcs/lib/Logger.hpp"
/*GET http://localhost:8080/
POST http://localhost:8080/api/login
PUT http://localhost:8080/api/user/42
DELETE http://localhost:8080/api/user/42*/


void GenerateSigeConf(const GlobalConfig const & GlobREF)
{
	std::vector<Server> root = GlobalConfig.getServers();
	std::vector<Location> Loc;
	std::vector<std::string> method = {"GET", "DELETE", "POST", "UKWN"}
	for (size_t i = 0; i < root.size(); ++i)
	{
		std::string location = "htpp:://" + root[i].getHost() + ":" + root[i].getPort();
		for (size_t y = 0; y < method.size(); ++y)
		{
			std::cout << method[y] << location << "\n";
		}
		Loc = root[i].location_;
		for(size_t f = 0)
	}

}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 2)
	{
		std::cout << "add parameters" << "\n\n";
		return 1;
	}
	GlobalConfig run;
	if (run.loadConfig(argv[1]) == false)
		return (1);
	GenerateSigeConf(run);
    return 0;
}
