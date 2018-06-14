#include "httpClientInterface.h"

int
ConnectionClient::post( const std::string& url, const std::string& body )
{
    httpsDisable( );
    sendPost( url, body );
    return 0;
}

int
ConnectionClient::post( const std::string& url,
                        const std::string& body,
                        const std::string& certPath )
{
    httpsDisable( );
    sendPost( url, body );
    return 0;
}

int
ConnectionClient::post( const std::string& url,
                        const std::string& body,
                        const std::map< std::string, std::string >& headerMap )
{
    httpsDisable( );
    for ( auto it : headerMap )
    {
        addHeader( it.first, it.second );
    }
    sendPost( url, body );
}

int
ConnectionClient::post( const std::string& url,
                        const std::string& body,
                        const std::map< std::string, std::string >& headerMap,
                        const std::string& certPath )
{
    httpsEnable( certPath );
    for ( auto it : headerMap )
    {
        addHeader( it.first, it.second );
    }
    sendPost( url, body );
}

int
ConnectionClient::get( const std::string& url )
{
    sendGet( url );
    return 0;
}

int
ConnectionClient::get( const std::string& url, const std::string& certPath )
{
    httpsEnable( certPath );
    sendGet( url );
    return 0;
}

int
ConnectionClient::get( const std::string& url,
                       const std::map< std::string, std::string >& headerMap )
{
    for ( auto it : headerMap )
    {
        addHeader( it.first, it.second );
    }
    sendGet( url );
    return 0;
}

int
ConnectionClient::get( const std::string& url,
                       const std::map< std::string, std::string >& headerMap,
                       const std::string& certPath )
{
    httpsEnable( certPath );
    for ( auto it : headerMap )
    {
        addHeader( it.first, it.second );
    }
    sendGet( url );
    return 0;
}
