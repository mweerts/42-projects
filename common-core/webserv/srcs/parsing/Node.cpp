#include "GlobalConfig.hpp"

/*♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
const Node* Node::findChildNode(const std::string& uri) const {
    std::map<std::string, Node*>::const_iterator it = route.find(uri);
    if (it != route.end())
        return it->second;
    else
        return NULL;
}
/*♡♡♡♡♡♡♡♡♡♡♡FORMATTER_PARAMATERS♡♡♡♡♡♡♡♡♡♡♡♡♡*/
/*♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
int Node::clearMap(void) {
    std::map<std::string, std::vector<std::string> >::iterator it =
        prmtrs.begin();

    for (; it != prmtrs.end(); ++it) {
        std::vector<std::string>& vec = it->second;
        if (!vec.empty()) {
            size_t       pos;
            std::string& sub = vec.back();
            if ((pos = sub.find(";")) != std::string::npos) {
                sub.erase(sub.find_last_not_of(";") + 1);
            } else {
                Logger::error() << "Syntax error at: " << sub
                                << " in parameter: " << it->first;
                return (1);
            }
        }
    }
    return (0);
}

void Node::addDefualtParm(void) {
    std::vector<std::string> vecRoot;
    vecRoot.push_back("./");
    prmtrs.insert(std::make_pair("root", vecRoot));
    if (this->name == "server") {
        std::vector<std::string> vec;
        vec.push_back("1048576");
        prmtrs.insert(std::make_pair("client_max_body_size", vec));
        std::vector<std::string> vecIp;
        vecIp.push_back("127.0.0.1");
        prmtrs.insert(std::make_pair("host", vecIp));
        std::vector<std::string> vecName;
        vecName.push_back("webserv");
        prmtrs.insert(std::make_pair("server_name", vecName));
    }
    if (this->name != "cgi-bin") {
        std::vector<std::string> vecAuto;
        vecAuto.push_back("off");
        prmtrs.insert(std::make_pair("autoindex", vecAuto));
        if (this->name != "server") {
            std::vector<std::string> vecMetho;
            vecMetho.push_back("GET");
            prmtrs.insert(std::make_pair("allow_methods", vecMetho));
        }
        std::vector<std::string> vecIndex;
        vecIndex.push_back("index.html");
        prmtrs.insert(std::make_pair("index", vecIndex));
    }
}
int Node::pushArgInMap(void) {
    std::vector<std::string>::iterator it = array.begin();
    std::string                        key;
    while (it != array.end()) {
        if (!it->empty() && (*it)[it->size() - 1] != ';')
            key = *it;
        if (key == "error_page" && it + 1 != array.end()) {
            std::vector<std::string>::iterator tmp = it + 1;
            while (tmp != array.end() && !tmp->empty() &&
                   (*tmp)[tmp->size() - 1] != ';') {
                key += " " + *tmp;
                it++;
                tmp++;
            }
        }
        if (it + 1 != array.end())
            ++it;
        std::vector<std::string>::iterator start = it;
        std::vector<std::string>::iterator end = it;
        while (it != array.end()) {
            end = it;
            if (!it->empty() && (*it)[it->size() - 1] == ';') {
                ++it;
                break;
            }
            ++it;
        }
        if (!key.empty())
            prmtrs.insert(
                std::make_pair(key, std::vector<std::string>(start, end + 1)));
        else if (key.empty()) {
            if (start != array.begin())
                Logger::error() << "Syntax error at: " << *(start - 1);
            else
                Logger::error() << "Syntax error";
            return (1);
        }
        key.clear();
    }
    if (clearMap() == 1)
        return (1);
    addDefualtParm();
    return (0);
}

/*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
void Node::printMap(void) const {
    Logger::info() << "Istruzzioni all'interno del nodo: " << name;
    std::map<std::string, std::vector<std::string> >::const_iterator stamp =
        prmtrs.begin();
    while (stamp != prmtrs.end()) {
        std::string              key = stamp->first;
        std::vector<std::string> value = stamp->second;
        std::cout << "La chiave: " << key << std::endl;
        std::cout << "I valori :";
        std::vector<std::string>::const_iterator val = value.begin();
        while (val != value.end()) {
            std::cout << " " << *val;
            val++;
        }
        std::cout << "\n\n";
        stamp++;
    }
}

void Node::printTree(void) const {
    std::cout << name << "\n";
    std::vector<std::string>::const_iterator it = array.begin();
    (void)it;
    for (size_t i = 0; i < children.size(); ++i) {
        bool isLast = (i == children.size() - 1);
        children[i].printSubtree("", isLast);
    }
}

void Node::printSubtree(const std::string& prefix, bool isLast) const {
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << name << "\n";
    std::vector<std::string>::const_iterator it = array.begin();
    (void)it;
    for (size_t i = 0; i < children.size(); ++i) {
        bool        childIsLast = (i == children.size() - 1);
        std::string newPrefix = prefix + (isLast ? "    " : "│   ");
        children[i].printSubtree(newPrefix, childIsLast);
    }
}
