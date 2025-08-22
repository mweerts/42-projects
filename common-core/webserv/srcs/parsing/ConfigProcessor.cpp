/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigProcessor.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebugle <lucas.lebugle@student.s19.be>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:53 by jfranco           #+#    #+#             */
/*   Updated: 2025/08/19 19:41:58 by jfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigProcessor.hpp"
#include "GlobalConfig.hpp"

/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ConfigProcessor::ConfigProcessor(const std::string& Path) : PathFile(Path) {
    std::cout << this->PathFile << "\n";
}

ConfigProcessor::ConfigProcessor() : PathFile("") {
    // std::cout << "Default constructor called" << std::endl;
    //  ctor
}

ConfigProcessor::ConfigProcessor(ConfigProcessor const& src) {
    // std::cout << "Copy constructor called" << std::endl;
    *this = src;
}

/*♡♡♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡*/
std::string ConfigProcessor::getPath(void) const {
    return (this->PathFile);
}

std::string ConfigProcessor::getBuffer(void) const {
    return (this->Buffer);
}

std::vector<Node> ConfigProcessor::getVectorOfServer(void) const {
    return this->tree;
}

std::map<int, Node> ConfigProcessor::getFullMap(void) const {
    return this->Servers;
}

std::vector<int> ConfigProcessor::getAllPorts(void) const {
    return this->allPort;
}

/*♡♡♡♡♡♡♡♡♡♡♡UTILS♡♡♡♡♡♡♡♡♡♡♡♡♡*/
void ConfigProcessor::prepareForCore(void) {
    std::vector<Node>::iterator it = tree.begin();
    while (it != tree.end()) {
        for (size_t i = 0; i < it->children.size(); ++i) {
            it->route.insert(
                std::make_pair(it->children[i].name, &it->children[i]));
        }
        ++it;
    }
    it = tree.begin();
    while (it != tree.end()) {
        std::map<std::string, std::vector<std::string> >::const_iterator
            listen = it->prmtrs.find("listen");
        if (listen != it->prmtrs.end() && !listen->second.empty()) {
            int               port;
            std::stringstream ss(listen->second[0]);
            ss >> port;
            this->allPort.push_back(port);
            this->Servers.insert(std::make_pair(port, *it));
        }
        ++it;
    }
}
void ConfigProcessor::printAllTree(void) const {
    std::vector<Node>::const_iterator it = tree.begin();
    while (it != tree.end()) {
        Logger::info() << "Schema Alberi e nodi";
        it->printTree();
        ++it;
    }
    std::cout << "\n\n";
    std::vector<Node>::const_iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        std::cout << "Genitore" << "\n";
        it_->printMap();
        std::cout << "Figli" << "\n";
        for (size_t i = 0; i < it_->children.size(); ++i) {
            it_->children[i].printMap();
        }
        ++it_;
    }
}

std::string ConfigProcessor::findRemplaceComment(std::string const& input,
                                                 std::string const& from,
                                                 std::string const& dilimiter,
                                                 std::string const& to) {
    std::string result;
    size_t      start = 0;
    size_t      pos, posEnd;
    if (from.empty() || dilimiter.empty())
        return input;

    while ((pos = input.find(from, start)) != std::string::npos &&
           (posEnd = input.find(dilimiter, pos)) != std::string::npos) {
        result.append(input, start, pos - start);
        result += to;
        start = posEnd + (dilimiter.length());
    }
    result.append(input, start, input.size() - start);
    pos = input.find(from, start);
    if (pos != std::string::npos) {
        result.append(input, start, pos - start);
        result += to;
    }
    return result;
}
/*♡♡♡♡♡♡♡♡♡♡♡VALIDATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
typedef void (Validator::*ValidateFunction)(const std::vector<std::string>&);

int ConfigProcessor::validateErrorPage(void) {
    size_t pos;
    // do nothung
    for (size_t i = 0; i < tree.size(); ++i) {
        std::map<std::string, std::vector<std::string> >::const_iterator it =
            tree[i].prmtrs.begin();
        while (it != tree[i].prmtrs.end()) {
            if ((pos = it->first.find("error", 0, 5)) != std::string::npos) {
                try {
                    this->valval.validateErrorPage(it->second);
                } catch (std::exception& e) {
                    Logger::error() << e.what() << tree[i].name;
                    return (1);
                }
            }
            it++;
        }
        for (size_t y = 0; y < tree[i].children.size(); ++y) {
            std::map<std::string, std::vector<std::string> >::const_iterator
                itChildren = tree[i].children[y].prmtrs.begin();
            while (itChildren != tree[i].children[y].prmtrs.end()) {
                if ((pos = itChildren->first.find("error", 0, 5)) !=
                    std::string::npos) {
                    try {
                        this->valval.validateErrorPage(itChildren->second);
                    } catch (std::exception& e) {
                        Logger::error() << e.what() << tree[i].children[y].name;
                        return (1);
                    }
                }
                ++itChildren;
            }
        }
    }
    return (0);
}
int ConfigProcessor::heandelError(
    ValidateFunction                                           fun,
    std::map<std::string, std::vector<std::string> >::iterator itPrmtrs,
    const std::string&                                         name) {
    try {
        Logger::info() << "Try validate: " << itPrmtrs->first << " in " << name;
        (this->valval.*fun)(itPrmtrs->second);
    } catch (Validator::DontValidIp& e) {
        Logger::error() << e.what() << " " << itPrmtrs->first;
        ;
        Logger::warning() << "Defalt ip set";
        itPrmtrs->second[0] = "127.0.0.1";
    } catch (Validator::PortAccessDeniedException& e) {
        Logger::error() << e.what() << " " << itPrmtrs->first;
        ;
        Logger::warning() << "Defalt port setting";
        itPrmtrs->second[0] = "8080";
    } catch (std::exception& e) {
        Logger::error() << e.what() << " " << itPrmtrs->first;
        return (1);
    }
    return (0);
}

int ConfigProcessor::validateForbiddenParameters(void) const {
    std::vector<Node>::const_iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        for (size_t i = 0; i < it_->children.size(); ++i) {
            if (verifyInvalidParamsInContext(it_->children[i].name,
                                             it_->children[i]) == 1)
                return (1);
            if (it_->children[i].name == "cgi-bin") {
                if (it_->children[i].prmtrs.count("cgi_ext") < 1) {
                    Logger::error()
                        << "missing cgi_ext in: " << it_->children[i].name;
                    return (1);
                }
                if (it_->children[i].prmtrs.count("cgi_path") < 1) {
                    Logger::error()
                        << "missing cgi_path in: " << it_->children[i].name;
                    return (1);
                }
                if (it_->children[i].prmtrs.count("root") < 1) {
                    Logger::error()
                        << "missing root in: " << it_->children[i].name;
                    return (1);
                }
				if ((it_->children[i].prmtrs.count("cgi_path") >= 1) && (it_->children[i].prmtrs.count("cgi_ext") >= 1))
				{
					std::vector<std::string> bin = it_->children[i].prmtrs.at("cgi_path");
					std::vector<std::string> ext = it_->children[i].prmtrs.at("cgi_ext");
					if (bin.size() != ext.size())
					{
						Logger::error()
						 << "Mismatch between CGI interpreters and extensions in configuration block: "
						 << it_->children[i].name
						 << ". Found " << bin.size() << " 'cgi_path' entries but " << ext.size()
						 << " 'cgi_ext' entries.";
		
						return (1);
					}
				}
            }
        }
        ++it_;
    }
    return (0);
}

int ConfigProcessor::verifyInvalidParamsInContext(const std::string& name,
                                                  const Node&        it) const {
    std::vector<std::string> vecNoAll;
    vecNoAll.push_back("listen");
    vecNoAll.push_back("host");
    vecNoAll.push_back("server_name");
	vecNoAll.push_back("upload_dir");
	vecNoAll.push_back("tmp_folder");
    //	vecNoAll.push_back("error_page");
    if (name == "cgi-bin") {
        vecNoAll.push_back("allow_methods");  // TODO: needs to be configured at the location level
        vecNoAll.push_back("autoindex"); // TODO: remove this, not allowed in cgi-bin
        vecNoAll.push_back("alias"); // TODO: remove this, not allowed in cgi-bin
        vecNoAll.push_back("return"); // TODO: remove this, not allowed in cgi-bin
    } else {
        vecNoAll.push_back("cgi_ext");
        vecNoAll.push_back("cgi_path");
    }
    for (size_t i = 0; i < vecNoAll.size(); i++) {
        std::map<std::string, std::vector<std::string> >::const_iterator
            itPrmtrs = it.prmtrs.find(vecNoAll[i]);
        if (itPrmtrs != it.prmtrs.end()) {
            Logger::error() << "prmtrs non alloewd here: " << name << " "
                            << itPrmtrs->first;
            return (1);
        }
    }
    vecNoAll.clear();
    return (0);
}

int ConfigProcessor::validateCgiBin(void) const {
    size_t                            count = 0;
    std::vector<Node>::const_iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        count = 0;
        for (size_t i = 0; i < it_->children.size(); ++i) {
            if (it_->children[i].name == "cgi-bin")
                count++;
        }
        if (count < 1) {
            Logger::error() << "Cgi-bin, is a mandatory parameter";
            return (1);
        }
        ++it_;
    }
    return (0);
}

void ConfigProcessor::heredityClientMaxBody(void) {
    std::vector<Node>::iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        std::map<std::string, std::vector<std::string> >::const_iterator param =
            it_->prmtrs.find("client_max_body_size");
        if (param != it_->prmtrs.end()) {
            for (size_t i = 0; i < it_->children.size(); ++i) {
                if (param != it_->prmtrs.end() &&
                    it_->children[i].name != "cgi-bin") {
                    it_->children[i].prmtrs.insert(*param);
                }
            }
        }
        ++it_;
    }
}

int ConfigProcessor::validateDifferentPortServer(void) const {
    std::vector<Node>::const_iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        std::map<std::string, std::vector<std::string> >::const_iterator
            listen = it_->prmtrs.find("listen");
        if (listen != it_->prmtrs.end()) {
            size_t i = 0;
            while (i < tree.size()) {
                size_t idx = std::distance(tree.begin(), it_);
                if (idx == i) {
                    i++;
                    continue;
                }
                std::map<std::string, std::vector<std::string> >::const_iterator
                    compare = tree[i].prmtrs.find("listen");
                if (compare != tree[i].prmtrs.end()) {
                    if (compare->second[0] == listen->second[0]) {
                        Logger::error()
                            << "Servers must have different listening to each "
                               "other the port is: "
                            << compare->second[0];
                        return (1);
                    }
                }
                i++;
            }
        }
        ++it_;
    }
    return (0);
}
static	int	isKnownParameter(const std::string& key, const std::string& nameREF)
{		
	Logger::warning() << "Parameter \"" << key << "\" in configuration section \"" << nameREF 
                  << "\" is not recognized. Press any key to continue, or 'n' to abort.";
	std::string in;
	std::cin >> in;
	if (in == "n")
		return 1;
	return 0;
}

int ConfigProcessor::validationParameters(void) {
    if (validateCgiBin() == 1)
        return (1);
    if (validateForbiddenParameters() == 1)
        return (1);
    heredityClientMaxBody();
    if (this->validateErrorPage() == 1)
        return (1);
    std::vector<Node>::iterator it_ = tree.begin();
    while (it_ != tree.end()) {
        std::map<std::string, std::vector<std::string> >::iterator itPrmtrs =
            it_->prmtrs.begin();
        if (it_->prmtrs.count("listen") < 1 ||
				it_->prmtrs.count("root") < 1) {
            Logger::error() << "listening port and root, is mandatory parameter";
            return (1);
        }
        if (it_->prmtrs.count("alias") != 0 ||
            it_->prmtrs.count("cgi_path") != 0 ||
            it_->prmtrs.count("cg_ext") != 0 ||
            it_->prmtrs.count("allow_methods") != 0) {
            Logger::error() << "forbitten prmtrs in server main";
            return (1);
        }
        while (itPrmtrs != it_->prmtrs.end()) {
            std::map<std::string, ValidateFunction>::iterator itFunc =
                this->valval.funcMap.find(itPrmtrs->first);
            if (itFunc != this->valval.funcMap.end()) {
                ValidateFunction func = itFunc->second;
                if (heandelError(func, itPrmtrs, it_->name) == 1)
                    return (1);
            }
			else if (itPrmtrs->first != "index" && itPrmtrs->first.rfind("error_page ", 0) != 0){
				if (isKnownParameter(itPrmtrs->first, it_->name) == 1){
					return 1;
				}
			}
            ++itPrmtrs;
        }
        std::vector<Node>::iterator itChild = it_->children.begin();
        while (itChild != it_->children.end()) {
            std::map<std::string, std::vector<std::string> >::iterator
                itPrmtrs = itChild->prmtrs.begin();
            while (itPrmtrs != itChild->prmtrs.end()) {
                std::map<std::string, ValidateFunction>::iterator itFunc =
                    this->valval.funcMap.find(itPrmtrs->first);
                if (itFunc != this->valval.funcMap.end()) {
                    ValidateFunction func = itFunc->second;
                    if (heandelError(func, itPrmtrs, itChild->name) == 1)
                        return (1);

                }
				else if (itPrmtrs->first != "index" && itPrmtrs->first.rfind("error_page ", 0) != 0){
					if (isKnownParameter(itPrmtrs->first, itChild->name) == 1){
						return 1;
					}
				}
                ++itPrmtrs;
            }
            itChild++;
        }
        ++it_;
    }
    if (validateDifferentPortServer() == 1)
        return (1);
    return (0);
}

void ConfigProcessor::RicorsiveTree(std::stringstream& sstoken, bool flags) {
    std::string token;
    char        c;
    if (!(sstoken >> token))
        return;
    sstoken >> std::ws;  // salta spazi bianchi (spazi, tab, newline)
    c = sstoken.peek();
    if (token == "server" && c == '{' && flags == true) {
        Logger::info() << "Push one tree";
        sstoken.get();
        Node root;
        root.name = token;          // salva il tipo di blocco
        treeParser(sstoken, root);  // costruisci il sottoalbero
        tree.push_back(root);       // aggiungi alla foresta
    }
    RicorsiveTree(sstoken);
}

void ConfigProcessor::treeParser(std::stringstream& sstoken, Node& current) {
    std::string token;
    std::string rest;

    while (sstoken >> token) {
        if (token == "location") {
            while (sstoken >> token && token != "{") rest = rest + token;
            if (token == "{") {
                Node child;
                child.name = rest;
                rest.clear();
                treeParser(sstoken, child);
                current.children.push_back(child);
                Logger::info() << "Push Node: " << child.name;
            } else {
                Logger::error() << "Bracket don't open corretly";
                return;
                // TODO: Da gestire corretamente;
            }
        }
        else {
            if (token == "}")
                return;
            current.array.push_back(token);
        }
    }
    return;
}

int ConfigProcessor::StreamErrorFind(std::stringstream& ss) const {
    if (ss.fail()) {
        Logger::error()
            << "Errore di formato o estrazione fallita (failbit set)";
        if (ss.eof()) {
            Logger::error()
                << "Si è raggiunta la fine dello stream (eofbit set)";
        }
        if (ss.bad()) {
            Logger::error() << "Errore grave di stream (badbit set)";
        }
        return (1);
    } else {
        Logger::info() << "Stream OK!";
    }
    return (0);
}
#include <sys/stat.h>

int ConfigProcessor::ValidationPath() const {
    const std::string exte = ".conf";
    size_t            posDoth = PathFile.rfind(exte);
    struct stat       sb;
    if (posDoth != std::string::npos) {
        if (posDoth == (PathFile.length() - exte.length())) {
            Logger::info() << "Valid extension, try open file";
            if (stat(PathFile.c_str(), &sb) == 0 && (sb.st_mode & S_IFDIR)) {
                Logger::error() << "the configuration must be a file";
                return (1);
            }
        } else {
            Logger::error() << "The file must end with '.conf'";
            return (1);
        }

    } else {
        Logger::error()
            << "The configuration file must have a '.conf' extension";
        return (1);
    }
    return (0);
}

bool ConfigProcessor::isIsolatedBrace(const std::string& str, size_t index) const {
    char c = str[index];
    if (c != '{' && c != '}')
        return false;
    if (index > 0) {
        char prev = str[index - 1];
        if (!isspace(prev))
            return false;
    }
    if (index + 1 < str.length()) {
        char next = str[index + 1];
        if (!isspace(next))
            return false;
    }
    return true;
}

int ConfigProcessor::checkBraces(const std::string& str) const {
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '{' || str[i] == '}') {
            if (!isIsolatedBrace(str, i)) {
                Logger::error() << "Brace is NOT isolated: ";
				return (1);
            }
        }
    }
	return (0);
}

#include <stack>

int ConfigProcessor::countBracket() const {
	std::stack<char> c;
    size_t  i = 0;
	std::string s = this->Buffer;
    if (s.size() <= 1)
        return 1;
     while (i < s.size())
     {
        if ( s[i] == '(' || s[i] == '[' || s[i] == '{')
        {
            c.push(s[i]);
        }
        if ( (s[i] == ')' || s[i] == ']' || s[i] == '}') && !c.empty())
        {
            if (s[i] == ')' && c.top() != '(')
            {
                return 1;
            }
            else if (s[i] == '}' && c.top() != '{')
            {
                return 1;
            }
            else if (s[i] == ']' && c.top() != '[')
            {
                return 1;
            }
            else if (s[i] == ')' && c.top() == '(')
            {
                c.pop();
            }
            else if (s[i] == '}' && c.top() == '{')
            {
                c.pop();
            }
            else if (s[i] == ']' && c.top() == '[')
            {
                c.pop();
            }
        }
        else if ( ( s[i] == ')' || s[i] == ']' || s[i] == '}' ) && c.size() == 0)
            return 1;
        i++;
     }
     if (c.size() == 0)	
         return 0;
     return 1;
}

int ConfigProcessor::recursiveMap(void) {
    std::vector<Node>::iterator it = tree.begin();
    while (it != tree.end()) {
        if (it->pushArgInMap() == 1) {
            return (1);
        }
        for (size_t i = 0; i < it->children.size(); ++i) {
            if (it->children[i].pushArgInMap() == 1)
                return (1);
        }
        ++it;
    }
    return (0);
}

static bool CheckFileStream(std::ifstream& file, const std::string& filename) {
    if (!file.is_open()) {
        Logger::error() << "Failed to open configuration file: " << filename;
        return false;
    }

    if (file.bad()) {
        Logger::error() << "I/O error while accessing file: " << filename;
        return false;
    }

    if (file.fail()) {
        Logger::error() << "Logical error on file stream for: " << filename;
        return false;
    }

    if (file.eof()) {
        Logger::error() << "End of file reached immediately for: " << filename;
        return false;
    }

    if (file.good())
        Logger::info() << "File stream is valid: " << filename;

    return true;
}

int ConfigProcessor::valideteSize(void) const {
    if (this->tree.size() < 1) {
        Logger::error() << "No found a valid config";
        return (1);
    }
	for (size_t i = 0; i < tree.size(); ++i)
	{
		for (size_t y = 0; y < tree[i].children.size(); ++y)
		{
			if (tree[i].children[y].children.size() > 0)
			{	
				Logger::error() << "Level 3 not allowed in this config";
				return (1);
			}
		}
	}
    return (0);
	
}

int ConfigProcessor::tokenize(void) {
    if (ValidationPath() == 1)
        return (1);
    std::ifstream file(this->PathFile.c_str());
    if (!CheckFileStream(file, this->PathFile)) {
        Logger::error() << "Error stream";
        file.close();
        return (1);
    }
    /* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
            // Use a stringstream to read the entire file content into memory
     ♡♡♡♡♡♡
     ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
   */
    std::stringstream ss;
    if (StreamErrorFind(ss) == 1)
        return (1);
    ss << file.rdbuf();
    file.close();
    this->Buffer = ss.str();
    /* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
         * Remove all comments from the buffer:
         * Look for each '#' character (start of comment)
         * and remove everything up to the newline character '\n'.
         * The `findRemplaceComment` function performs this operation.
     ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
   */
    this->Buffer = findRemplaceComment(this->Buffer, "#", "\n", "\n");
    if (countBracket() == 1)
	{
		Logger::error() << "Close Bracket pls";
        return (1);
	}
    /* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
     * Create a stringstream from the cleaned buffer, allowing tokenization
     using >>.
     * Then pass it to treeParser for building the configuration tree structure.
     ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
   */

    std::stringstream tokenStream(this->Buffer);
    if (StreamErrorFind(tokenStream) == 1)
        return (1);
    RicorsiveTree(tokenStream);
    if (valideteSize() == 1)
        return (1);
    if (recursiveMap() == 1)
        return (1);
    if (validationParameters() == 1)
        return (1);
    prepareForCore();
    return (0);
}

/*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ConfigProcessor& ConfigProcessor::operator=(ConfigProcessor const& rhs) {
    // std::cout << "Copy assignment operator called" << std::endl;
    if (this != &rhs) {
        // this->_n = rhs.getValue();
    }
    return *this;
}

/*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ConfigProcessor::~ConfigProcessor() {
    // std::cout << "Destructor called" << std::endl;
    //  dtor
}

std::ostream& operator<<(std::ostream& o, const ConfigProcessor& rhs) {
    o << "File Conf: " << rhs.getPath() << "\n"
      << "Buffer: " << rhs.getBuffer();
    return o;
}
