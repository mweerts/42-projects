/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlobalConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfranco <jfranco@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:27:30 by jfranco           #+#    #+#             */
/*   Updated: 2025/07/15 18:13:10 by jfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALCONFIG_HPP
#define GLOBALCONFIG_HPP

#include "ConfigProcessor.hpp"

struct CgiBin {
    CgiBin(const std::map<std::string, std::vector<std::string> >& passprmtrs,
           const std::string& nameLocation);
    std::string                                      name;
    std::map<std::string, std::vector<std::string> > prmtrs;
    CgiBin();

    const std::vector<std::string>* getPath(void) const;
    const std::vector<std::string>* getExt(void) const;
    const std::string*              getRoot(void) const;
};

class Location {
   private:
    std::string name;

   public:
    std::map<std::string, std::vector<std::string> > prmtrs;
    const std::string&                               getName(void);
    Location(const std::map<std::string, std::vector<std::string> >& passprmtrs,
             const std::string& nameLocation);
    size_t				getClientMaxBodySize( void )const;
	bool				getAutoIndex( void ) const;
	const std::string*	getRoot( void ) const;
	const std::string*	getIndex( void ) const;
	bool				getMethodIsAllowed(const std::string& method) const;
	const std::string*	getAlias( void ) const;
	const std::string*	getReturn( void ) const;
    size_t				getHost( void )const;
    int					getPort( void ) const;
};

class ServerConfig {
   private:
    std::string                                      name;
    std::map<std::string, std::vector<std::string> > prmtrs;
    CgiBin                                           CGIloc;
    bool                                             Cgi;

   public:
    ServerConfig() {};
    ServerConfig(
        const std::map<std::string, std::vector<std::string> >& passprmtrs,
        const std::string&                                      nameServer);

    const std::string& getName(void) const;
    const std::string  getHost(void) const;
    size_t             getClientMaxBodySize(void) const;
    bool               getAutoIndex(void) const;
    const std::string  getRoot(void) const;
    const std::string* getIndex(void) const;
    int                getPort(void) const;
    const Location&    getLocation(const std::string& uri) const;
    const std::string* getErrorPageLocation(const std::string& uri,
                                            const std::string& nbrError) const;
    const std::string* getErrorPage(const std::string& nbrError) const;
    void               setCgi(const CgiBin& add);
    const CgiBin&      getCgiBin(void) const;

    std::map<std::string, Location> route;
    std::vector<Location>           location_;

    class NotFoundUri : public std::exception {
       public:
        virtual const char* what() const throw();  // dichiarazione
    };
};

class GlobalConfig {
   public:
    GlobalConfig() {};
    GlobalConfig(const ConfigProcessor& Parser);
    GlobalConfig(const GlobalConfig& other);
    void CreateServerAndLocation(void);
    virtual ~GlobalConfig();

    //		//♡♡♡ Returns the custom error page path for a given port and
    // URI, checking location-level config first.♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
    //		//♡♡♡ Falls back to server-level config if no match is found in
    // the location.                           ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
    // const std::string* getErrorPage(int port, const std::string& error,
    //                                 const std::string& uri) const;
    //
    //		//♡♡♡ Returns the custom error page path for a given port, using
    // only server-level configuration.♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
    //		//♡♡♡ Does not consider URI-specific location blocks.
    // ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
    // const std::string* getErrorPage(int port, const std::string& error)
    // const;
    //
    const std::vector<ServerConfig> getServers() const;

   private:
    std::vector<Node>         tree;
    std::vector<int>          allPort;
    std::map<int, Node>       MapNode;
    std::vector<ServerConfig> servers;
};

#endif
