#ifndef CONFIGPROCESSOR_H
#define CONFIGPROCESSOR_H


#include "../../../includes/Logger.hpp"
#include <vector>
#include <algorithm>  // per std::find
#include <iostream>
#include <string>
#include <sstream>   // ← necessario per std::stringstream
#include <iostream>  // ← se usi std::cout
#include <fstream>   // ← se usi std::ifstream
#include <map>   // ← se usi std::map
#include <cstdlib> // necessario per exit()
//

//typedef void (Validator::*)(std::vector<std::string>&)
//
#define MAX_BODY_SIZE 524288000

struct Validator
{
    typedef void (Validator::*ValidateFunction)(const std::vector<std::string>&);

       /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	Validator(); // ctor
   std::map<std::string, ValidateFunction> funcMap;
		void	(Validator::*_FunPTR[12])(std::vector<std::string>&);

       /*♡♡♡♡♡♡♡♡♡♡♡FT_VALIDATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
		void	validateIp(const std::vector<std::string>& prmtrs);
		void	validateCgiPath(const std::vector<std::string>& prmtrs);
		void	validateListen(const std::vector<std::string>& prmtrs);
		void	validateServerName(const std::vector<std::string>& prmtrs);
		void	validateErrorPage(const std::vector<std::string> & prmtrs);
		void	validateClienMaxBody(const std::vector<std::string>& prmtrs);
		void	validatePath(const std::string& prmtrs);
		void	validateRoot(const std::vector<std::string>& prmtrs);
		void	validateAutoIndex(const std::vector<std::string>& prmtrs);
		void	validateMethods(const std::vector<std::string>& prmtrs);
		void	validateCgiExt(const std::vector<std::string>& prmtrs);

       /*♡♡♡♡♡♡♡♡♡♡♡EXCPTION♡♡♡♡♡♡♡♡♡♡♡♡♡*/
		class PortAccessDeniedException : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class unknownMethods : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class ToManyDoth : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class OutOfRange : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class InvalidCharEx: public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class VectorSizeToHight : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class VectorSizeToLow : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		class  DontValidIp : public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
		
		class  Empty: public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};

		class  onlyDigit: public std::exception 
		{
			public:
				virtual const char* what() const throw();  // dichiarazione
		};
};

struct	Node
{
	std::string name;
	std::vector<std::string> array;
	std::vector<Node> children;
	std::map<std::string, std::vector<std::string> > prmtrs; 
	std::map<std::string, Node*> route;
       /*♡♡♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡*/
/*♡♡♡♡♡♡♡♡♡♡♡ FIND CHILD NODE ♡♡♡♡♡♡♡♡♡♡♡*/
/**
 * ♡ Searches among this Node’s children for a child Node matching the given URI ♡
 * ♡ 
 * ♡ @param uri The URI string to look for among children ♡
 * ♡ @return A pointer to the const child Node if found, otherwise nullptr ♡
 */
	const Node* findChildNode( const std::string& uri) const;

       /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
       /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/
       /*♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡*/

	void	printMap(void)const;

	void	printSubtree(const std::string &prefix, bool isLast) const;
	void	printTree() const ;
	void	addDefualtParm(void);
	int		pushArgInMap( void );
	int		clearMap( void );
};

typedef void (Validator::*ValidateFunction)(const std::vector<std::string>&);
 
class ConfigProcessor
{
   public:
       /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
       ConfigProcessor(const std::string& Path);  //cannon
       ConfigProcessor(ConfigProcessor const & src);   //Cannon
 
       /*♡♡♡♡♡♡♡♡♡♡♡GETTER♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	   std::string	getPath( void ) const;
	   std::string	getBuffer( void ) const;

        // ♡ Returns a const reference to the main vector of server nodes ♡
        const std::vector<Node>& getVectorOfServer(void) const;

        // ♡ Returns a const reference to the complete map of servers indexed by port ♡
        const std::map<int, Node*>& getFullMap(void) const;

        // ♡ Returns a const pointer to the map of routes (uri -> Node*) for a server by its port ♡
        // ♡ Returns nullptr if the port does not exist ♡
        const std::map<std::string, Node*>* getMapOfOneServer(int port) const;

        // ♡ Returns a const pointer to the route node specified by port and URI ♡
        // ♡ Returns nullptr if not found .
        const Node* getRouteNode(const std::string& port, const std::string& uri) const;
        const Node* getRouteNode(int port, const std::string& uri) const;

        // ♡ Returns a const pointer to the server node associated with the specified port ♡
        // ♡ Returns nullptr if the port does not exist ♡
        const Node* getServerNode(int port) const;

        // ♡ Returns a const reference to the vector containing all configured server ports ♡
        const std::vector<int>& getAllPorts() const;

        // ♡ Checks whether a given port is present among the configured ports ♡
        bool hasPort(int port);

        // ♡ Returns a const pointer to the vector of strings associated with a key (parameter) for a server specified by port ♡
        // ♡ Returns nullptr if the port or key do not exist ♡
        const std::vector<std::string>* getParam(int port, const std::string& key) const;

        // ♡♡♡ Returns a const pointer to the vector of strings associated with a key (parameter) for a route specified by port and URI ♡
        // ♡♡♡ Returns nullptr if the port, URI, or key do not exist ♡
        const std::vector<std::string>* getParam(int port, const std::string& uri, const std::string& key) const;

		//♡♡♡ Returns the custom error page path for a given port and URI, checking location-level config first.♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
		//♡♡♡ Falls back to server-level config if no match is found in the location.                           ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
		const std::string*	getErrorPage(int port, const std::string& error, const std::string& uri) const;

		//♡♡♡ Returns the custom error page path for a given port, using only server-level configuration.♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
		//♡♡♡ Does not consider URI-specific location blocks.                                            ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
		const std::string*	getErrorPage(int port, const std::string& error ) const;

	   void			printAllTree( void ) const;


       /*♡♡♡♡♡♡♡♡♡♡♡FT♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	   int	tokenize( void );

       ConfigProcessor& operator=(ConfigProcessor const & rsh);    //Cannon
 
       /*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
       virtual ~ConfigProcessor(); //Cannon
  private:
	   struct Validator valval;
       ConfigProcessor();  //cannon
	   const std::string	PathFile;
	   std::string		Buffer;
	   std::vector<Node> tree;
	   std::map<int, Node*> Servers;
	   std::vector<int>		allPort;
       /*♡♡♡♡♡♡♡♡♡♡♡FT♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	   std::string findRemplaceComment(std::string const& input,
			   std::string const& from, std::string const& dilimiter,
			   std::string const& to);  
	   void	RicorsiveTree(std::stringstream& sstoken, bool flags = true);
	   void	treeParser(std::stringstream& sstoken, Node& token);
	   void prepareForCore( void );
	   void	heredityClientMaxBody( void );
	   int	recursiveMap( void );
       /*♡♡♡♡♡♡♡♡♡♡♡FT_MSG_ERROR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	   int	StreamErrorFind(std::stringstream& ss) const;
       /*♡♡♡♡♡♡♡♡♡♡♡FT_VALIDATION♡♡♡♡♡♡♡♡♡♡♡♡♡*/
	   int		validateForbiddenParameters( void ) const;
	   int		verifyInvalidParamsInContext(const std::string& name, const Node &it) const;
	   int		validateDifferentPortServer( void ) const;
	   int		ValidationPath( void ) const;
	   int		validateCgiBin( void ) const;
	   int		validateErrorPage( void );
		int valideteSize( void ) const;
		int	countBracket() const;
		int	validationParameters( void );
		int	heandelError(ValidateFunction fun, std::map<std::string, std::vector<std::string> >::iterator itPrmtrs, const std::string &name);
       /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/

};
 std::ostream &operator<<(std::ostream &o, const ConfigProcessor &rhs);
#endif // CONFIGPROCESSOR_H

