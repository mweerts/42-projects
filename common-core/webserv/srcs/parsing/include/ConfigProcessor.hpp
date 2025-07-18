#ifndef CONFIGPROCESSOR_H
#define CONFIGPROCESSOR_H

#include <algorithm>  // per std::find
#include <cstdlib>    // necessario per exit()
#include <fstream>    // ← se usi std::ifstream
#include <iostream>
#include <iostream>  // ← se usi std::cout
#include <map>       // ← se usi std::map
#include <sstream>   // ← necessario per std::stringstream
#include <string>
#include <vector>

#include "../../../includes/Logger.hpp"
//

// typedef void (Validator::*)(std::vector<std::string>&)
//
#define MAX_BODY_SIZE 524288000

struct Validator {
    typedef void (Validator::*ValidateFunction)(
        const std::vector<std::string>&);

    /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    Validator();  // ctor
    std::map<std::string, ValidateFunction> funcMap;
    void (Validator::* _FunPTR[12])(std::vector<std::string>&);

    /*♡♡♡♡♡♡♡♡♡♡♡FT_VALIDATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	void	validateUploadDir(const std::vector<std::string>& prmtrs);
    void validateIp(const std::vector<std::string>& prmtrs);
    void validateCgiPath(const std::vector<std::string>& prmtrs);
    void validateListen(const std::vector<std::string>& prmtrs);
    void validateServerName(const std::vector<std::string>& prmtrs);
    void validateErrorPage(const std::vector<std::string>& prmtrs);
    void validateClienMaxBody(const std::vector<std::string>& prmtrs);
    void validatePath(const std::string& prmtrs);
    void validateRoot(const std::vector<std::string>& prmtrs);
    void validateAutoIndex(const std::vector<std::string>& prmtrs);
    void validateMethods(const std::vector<std::string>& prmtrs);
    void validateCgiExt(const std::vector<std::string>& prmtrs);

    /*♡♡♡♡♡♡♡♡♡♡♡EXCPTION♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    class PortAccessDeniedException : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class unknownMethods : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class ToManyDoth : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class OutOfRange : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class InvalidCharEx : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class VectorSizeToHight : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class VectorSizeToLow : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
    class DontValidIp : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };

    class Empty : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };

    class onlyDigit : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
};

struct Node {
    std::string                                      name;
    std::vector<std::string>                         array;
    std::vector<Node>                                children;
    std::map<std::string, std::vector<std::string> > prmtrs;
    std::map<std::string, Node*>                     route;
    /*♡♡♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    /*♡♡♡♡♡♡♡♡♡♡♡ FIND CHILD NODE ♡♡♡♡♡♡♡♡♡♡♡*/
    /**
     * ♡ Searches among this Node’s children for a child Node matching the given
     * URI ♡ ♡ ♡ @param uri The URI string to look for among children ♡ ♡
     * @return A pointer to the const child Node if found, otherwise nullptr ♡
     */
    const Node* findChildNode(const std::string& uri) const;

    /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/

    void printMap(void) const;

    void printSubtree(const std::string& prefix, bool isLast) const;
    void printTree() const;
    void addDefualtParm(void);
    int  pushArgInMap(void);
    int  clearMap(void);
};

typedef void (Validator::*ValidateFunction)(const std::vector<std::string>&);

class ConfigProcessor {
   public:
    /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    ConfigProcessor(const std::string& Path);     // cannon
    ConfigProcessor(ConfigProcessor const& src);  // Cannon

    /*♡♡♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    std::string getPath(void) const;
    std::string getBuffer(void) const;

    std::vector<Node> getVectorOfServer(void) const;

    std::map<int, Node> getFullMap(void) const;

    std::vector<int> getAllPorts() const;

    void printAllTree(void) const;

    /*♡♡♡♡♡♡♡♡♡♡♡FT♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    int tokenize(void);

    ConfigProcessor& operator=(ConfigProcessor const& rsh);  // Cannon

    /*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    virtual ~ConfigProcessor();  // Cannon
   private:
    struct Validator valval;
    ConfigProcessor();  // cannon
    const std::string   PathFile;
    std::string         Buffer;
    std::vector<Node>   tree;
    std::map<int, Node> Servers;
    std::vector<int>    allPort;
    /*♡♡♡♡♡♡♡♡♡♡♡FT♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    std::string findRemplaceComment(std::string const& input,
                                    std::string const& from,
                                    std::string const& dilimiter,
                                    std::string const& to);
    void        RicorsiveTree(std::stringstream& sstoken, bool flags = true);
    void        treeParser(std::stringstream& sstoken, Node& token);
    void        prepareForCore(void);
    void        heredityClientMaxBody(void);
    int         recursiveMap(void);
    /*♡♡♡♡♡♡♡♡♡♡♡FT_MSG_ERROR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    int StreamErrorFind(std::stringstream& ss) const;
    /*♡♡♡♡♡♡♡♡♡♡♡FT_VALIDATION♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	int	checkBraces(const std::string& str) const;
	bool	isIsolatedBrace(const std::string& str, size_t index)	const;
    int validateForbiddenParameters(void) const;
    int verifyInvalidParamsInContext(const std::string& name,
                                     const Node&        it) const;
    int validateDifferentPortServer(void) const;
    int ValidationPath(void) const;
    int validateCgiBin(void) const;
    int validateErrorPage(void);
    int valideteSize(void) const;
    int countBracket() const;
    int validationParameters(void);
    int heandelError(
        ValidateFunction                                           fun,
        std::map<std::string, std::vector<std::string> >::iterator itPrmtrs,
        const std::string&                                         name);
    /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
};
std::ostream& operator<<(std::ostream& o, const ConfigProcessor& rhs);
#endif  // CONFIGPROCESSOR_H
