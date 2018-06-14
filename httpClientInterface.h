#ifndef HTTPCLIENTINTERFACE_H
#define HTTPCLIENTINTERFACE_H

#include "Socket/httpClient.h"

#include <map>

class ConnectionClient : private HttpClient
{
public:
    int post( const std::string& url, const std::string& body );
    int post( const std::string& url, const std::string& body, const std::string& certpath );
    int post( const std::string& url,
              const std::string& body,
              const std::map< std::string, std::string >& );
    int post( const std::string& url,
              const std::string& body,
              const std::map< std::string, std::string >&,
              const std::string& certPath );
    int get( const std::string& url );
    int get( const std::string& url, const std::string& certpath );
    int get( const std::string& url, const std::map< std::string, std::string >& );
    int get( const std::string& url,
             const std::map< std::string, std::string >&,
             const std::string& certPath );

private:
};

#endif
