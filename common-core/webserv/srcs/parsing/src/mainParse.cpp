#include "../include/ConfigProcessor.hpp"
#include "../../../includes/Logger.hpp"
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
		// test getter
	//	const std::vector<std::string> *info = conf.getParam(8080, "/happie", "allow_methods");
	//	if (!info)
	//		return (1);
		const std::string *errorPage = conf.getErrorPage(8000, "505", "/tours");
		if (!errorPage)
		{
			Logger::error() << "non esiste";
			return (1);
		}
		std::cout << *errorPage << "\n";
	//	std::vector<std::string>::const_iterator it = info->begin();
	//	while (it != info->end())
	//	{
	//		std::cout << *it << "\n\n";
	//		it++;
	//	}
	}

	return (0);
}
