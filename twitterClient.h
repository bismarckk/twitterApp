#ifndef TWITTER_H
#define TWITTER_H

#include "Socket/IHttpClient.h"
#include "TwitsContainer/twittsContainer.h"
#include "rapidjson/include/rapidjson/document.h"

#include <map>
#include <string>

enum ERRORS
{
    BAD_AUTHENTICATION = 1,
    STRING_IS_EMPTY = 2
};

class Twitter
{
public:
    Twitter( );
    TwittsContainer getTwitts( int count, const std::string& name );
    int sendTwitt( const std::string& twittText );
    int setClientKeyAndSecret( const std::string& key, const std::string& secret );
    int setOauthTokenAndSecret( const std::string& token, const std::string& secret );
    int setCertPath( const std::string& certPath );
    int setHttpClient( IHttpClient* client );

private:
    std::string authAppOnly( );
    std::string generateToken( );
    std::string createSignature( const std::string& tmpURL );
    int requestToken( );
    std::string authorizationHeader( );
    int requestAccessToken( );

private:
    std::map< std::string, std::string > m_signature;
    IHttpClient* m_httpClient;
    TwittsContainer m_twitts;
    std::string m_certPath;
    std::string m_clientKey;
    std::string m_clientSecret;
    std::string m_oauthTokenSecret;
    std::string m_oauthToken;
    std::string m_token;
    std::string m_oauthTokenFromRequest;
    std::string m_signingKey;
    std::string m_userToken;
    std::string m_userOauthTokenSecret;
};

#endif
