#ifndef IHTTPCLIENT_H
#define IHTTPCLIENT_H

#include <string>
class IHttpClient
{
public:
    virtual ~IHttpClient( ) = default;
    virtual void sendGet( const std::string& url ) = 0;
    virtual void addHeader( const std::string& key, const std::string& value ) = 0;
    virtual void sendPost( const std::string& url, const std::string& body ) = 0;
    virtual const std::string& getData( ) const = 0;
    virtual const std::string& getHeader( ) const = 0;
    virtual void httpsEnable( const std::string& certPath ) = 0;
    virtual void httpsDisable( ) = 0;
};
#endif
