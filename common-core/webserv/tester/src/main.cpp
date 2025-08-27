

#include "../../srcs/parsing/GlobalConfig.hpp"
#include "../../srcs/parsing/ConfigProcessor.hpp"
#include "../../srcs/parsing/GlobalConfig.hpp"
#include "../../includes/Logger.hpp"
#include <fstream>

/*GET http://localhost:8080/
POST http://localhost:8080/api/login
PUT http://localhost:8080/api/user/42
DELETE http://localhost:8080/api/user/42*/
			//
void ft_ciclo_di_copia(std::ofstream &ofREF, const std::vector<std::string> & vecREF, std::string url, std::string separator1, std::string separator2)
{
	for (size_t i = 0; i < vecREF.size(); ++i)
	{
		ofREF << url << separator1 << vecREF[i] << separator2;
	}
}

void ft_ciclo_di_copia(std::ofstream &ofREF, const std::vector<std::string> & vecREF, std::string url, std::string separator1, std::string separator2, const std::vector<std::string> vecREF2)
{
	for (size_t i = 0; i < vecREF.size(); ++i)
	{
		for (size_t y = 0; y < vecREF2.size(); ++y)
		{
			//ofREF << url << << separator1 << vecREF[i] << separator2;
			if (vecREF2[y] == "POST")
			{
				separator2 = " <./file_40mb.txt\n";
			}
			ofREF << url << vecREF[i] << separator1 << vecREF2[y] << separator2;
		}
	}
}
void ft_ciclo_cgi(std::ofstream &ofREF, const std::vector<std::string> & vecREF, std::string url, std::string separator1, std::string separator2, const std::string stringREF)
{
		for (size_t y = 0; y < vecREF.size(); ++y)
		{
			ofREF <<  url << vecREF[y] << separator1 << stringREF << separator2;
		}
}

void GenerateCgiBin(const GlobalConfig & GlobREF)
{
	std::ofstream cgi("urlCGI.txt");

	std::vector<ServerConfig> root = GlobREF.getServers();
	std::vector<Location> Loc;
	std::vector<std::string> cgi_bin = {"/cgi-bin/log-viewer.py", "/cgi-bin/slow_cgi.py", "/cgi-bin"};
	for (size_t i = 0; i < root.size(); ++i)
	{
		int port = root[i].getPort();
		std::stringstream ss;
		ss << "http://" << root[i].getHost() << ":" << port;
		std::string urlMain = ss.str();
		ft_ciclo_cgi(cgi, cgi_bin, urlMain," ",  "\n", "");
	}
	cgi.close();
}

void GenerateSigeConfBasic(const GlobalConfig & GlobREF)
{
	std::ofstream basic("urlBasic.txt");

	std::vector<ServerConfig> root = GlobREF.getServers();
	std::vector<Location> Loc;
	std::vector<std::string> method = {"", "POST"};
	std::vector<std::string> cgi_bin = {"/cgi-bin/log-viewer.py", "/cgi-bin/slow_cgi.py", "/cgi-bin"};
	for (size_t i = 0; i < root.size(); ++i)
	{
		//std::string urlMain = "htpp:://" + root[i].getHost() + ":" + root[i].getPort();
		int port = root[i].getPort();
		std::stringstream ss;
		ss << "http://" << root[i].getHost() << ":" << port;
		std::string urlMain = ss.str();
		basic << urlMain << "/ " << "\n";
		ft_ciclo_cgi(basic, cgi_bin, urlMain," ",  "\n", "");
		Loc = root[i].location_;
		for(size_t f = 0; f < Loc.size(); ++f)
		{
			for (size_t l = 0; l < method.size(); ++l)
			{
				if (Loc[f].getMethodIsAllowed(method[l]) == true)
				{
					basic << urlMain << Loc[f].getName() << " " << method[l] << " <./file_40mb.txt\n";
				}
			}
		}
	}
	basic.close();

}

void GenerateSigeConfAdvance(const GlobalConfig & GlobREF)
{
	std::ofstream full("urlFull.txt");

	if (!full.is_open()) 
	{
		std::cerr << "Errore apertura file di output\n";
		return;
	}
	std::vector<std::string> static_paths = {"/index.html", "/css/global.css", "/img/logo.png", "/empty.html","/upload.html"};
	std::vector<std::string> error_paths = {"/doesnotexist", "/404", "/invalid/path"};
	std::vector<std::string> query_paths = {"/search?q=test", "/user?id=42", "/api/data?page=1"};
	std::vector<std::string> cgi_bin = {"/cgi-bin/log-viewer.py", "/cgi-bin/slow_cgi.py", "/cgi-bin"};
	std::vector<ServerConfig> root = GlobREF.getServers();
	std::vector<std::string> method = {"", "POST"};
	std::vector<Location> Loc;
	for (size_t i = 0; i < root.size(); ++i)
	{
		int port = root[i].getPort();
		std::stringstream ss;
		ss << "http://" << root[i].getHost() << ":" << port;
		std::string urlMainServer = ss.str();

		ft_ciclo_di_copia(full, method, urlMainServer,"/ ",  "\n");
		ft_ciclo_cgi(full, cgi_bin, urlMainServer," ",  "\n", method[0]);
		ft_ciclo_di_copia(full, static_paths, urlMainServer, " ", "\n", method);
		ft_ciclo_di_copia(full, error_paths, urlMainServer," ", "\n", method);
		Loc = root[i].location_;
		full << urlMainServer << "/\n";
		for(size_t f = 0; f < Loc.size(); ++f)
		{
			std::string uriloc = urlMainServer + Loc[f].getName();
			ft_ciclo_di_copia(full, method, uriloc, " ", "\n");
			ft_ciclo_di_copia(full, static_paths, uriloc, " ", "\n", method);
			ft_ciclo_di_copia(full, error_paths, uriloc, " ", "\n", method);
		}
	}
	full.close();

}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "add parameters" << "\n\n";
		return 1;
	}
	GlobalConfig run;
	if (run.loadConfig(argv[1]) == false)
		return (1);
	GenerateSigeConfAdvance(run);
	GenerateSigeConfBasic(run);
	GenerateCgiBin(run);
    return 0;
}
