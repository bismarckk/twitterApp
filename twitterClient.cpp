#include "twitterClient.h"
#include "EncodeFunctions/encodeFunctions.h"
#include "httpConstans.h"
#include "oauthConstans.h"
extern "C" {
#include "hmacSha1/src/hmac/hmac.h"
}

#include <ctime>
#include <iostream>
#include <map>
#include <random>

using namespace Oauth;

Twitter::Twitter( ) : m_httpClient( ) {}

int
Twitter::setHttpClient( IHttpClient* client )
{
    m_httpClient = client;
    return 0;
}

int
Twitter::setClientKeyAndSecret( const std::string& key, const std::string& secret )
{
    if ( key.empty( ) || secret.empty( ) )
    {
        return STRING_IS_EMPTY;
    }
    m_clientKey = key;
    m_clientSecret = secret;
    return 0;
}

int
Twitter::setOauthTokenAndSecret( const std::string& token, const std::string& secret )
{
    if ( token.empty( ) || secret.empty( ) )
    {
        return STRING_IS_EMPTY;
    }
    m_oauthTokenSecret = secret;
    m_oauthToken = token;
    return 0;
}

int
Twitter::setCertPath( const std::string& certPath )
{
    if ( certPath.empty( ) )
    {
        return STRING_IS_EMPTY;
    }
    m_certPath = certPath;
    return 0;
}

TwittsContainer
Twitter::getTwitts( int count, const std::string& name )
{
    if ( "" != m_clientKey && "" != m_clientSecret && "" != m_certPath )
    {
        m_token = authAppOnly( );
    }
    m_httpClient->httpsEnable( m_certPath );
    m_httpClient->addHeader( USERAGENT, "AppConnectWithTwitter" );
    m_httpClient->addHeader( ACCEPTENCODING, "json" );
    m_httpClient->addHeader( AUTHORIZATION, m_token );
    std::string tmpURL = "api.twitter.com/1.1/statuses/user_timeline.json?count=";
    tmpURL += std::to_string( count );
    tmpURL += "&screen_name=";
    tmpURL += name;
    m_httpClient->sendGet( tmpURL );
    rapidjson::Document doc;
    doc.Parse( m_httpClient->getData( ).c_str( ) );
    TwittsContainer twitts( m_httpClient->getData( ) );
    return twitts;
}

// sending post to twitt use signingKey (app secret and user oauth secret)
int
Twitter::sendTwitt( const std::string& twittText )
{
    if ( m_userToken.empty( ) )
    {
        requestToken( );
        requestAccessToken( );
        m_signature[Oauth::TOKEN] = m_userToken;
        m_signature[CONSUMERKEY] = m_clientKey;
        m_signature[SIGNATUREMETHOD] = "HMAC-SHA1";
        m_signature[VERSION] = "1.0";
        m_signingKey = m_clientSecret + "&" + m_userOauthTokenSecret;
    }
    const std::string TMPURL = "api.twitter.com/1.1/statuses/update.json";
    std::time_t t = std::time( 0 );
    m_signature[NONCE] = generateToken( );
    m_signature[TIMESTAMP] = std::to_string( t );
    m_signature["status"] = twittText;
    m_signature[SIGNATURE] = createSignature( TMPURL );
    m_signature.erase( "status" );

    m_httpClient->httpsEnable( m_certPath );
    m_httpClient->addHeader( AUTHORIZATION, authorizationHeader( ) );
    m_httpClient->sendPost( TMPURL + "?status=" + encodePercent( twittText ), "" );
    rapidjson::Document doc;
    doc.Parse( m_httpClient->getData( ).c_str( ) );
    m_signature.erase( SIGNATURE );
    // if(doc.HasMember(""))
    if ( doc.HasMember( "errors" ) )
    {
        std::cout << m_httpClient->getData( );
        return BAD_AUTHENTICATION;
    }
    return 0;
}

// Get oauth token needed to make url for user
// use signingKey (app secret and app oauth secret)
int
Twitter::requestToken( )
{
    std::string tmpURL = "api.twitter.com/oauth/request_token";
    m_signature.clear( );
    std::time_t t = std::time( 0 );
    m_signingKey = m_clientSecret + "&" + m_oauthTokenSecret;
    m_signature[TOKEN] = m_oauthToken;
    m_signature[CONSUMERKEY] = m_clientKey;
    m_signature[NONCE] = generateToken( );
    m_signature[SIGNATUREMETHOD] = "HMAC-SHA1";
    m_signature[TIMESTAMP] = std::to_string( t );
    m_signature[VERSION] = "1.0";
    m_signature[CALLBACK] = "oob";
    m_signature[SIGNATURE] = createSignature( tmpURL );

    m_httpClient->httpsEnable( m_certPath );
    m_httpClient->addHeader( AUTHORIZATION, authorizationHeader( ) );
    m_httpClient->sendPost( tmpURL, "" );
    rapidjson::Document doc;
    doc.Parse( m_httpClient->getData( ).c_str( ) );
    if ( doc.HasMember( "errors" ) )
    {
        std::cout << m_httpClient->getData( );
        return BAD_AUTHENTICATION;
    }

    std::string oauthToken = m_httpClient->getData( );
    size_t found = oauthToken.find( "oauth_token=" );
    size_t endToken = oauthToken.find( "&", found );
    if ( found != std::string::npos )
    {
        m_oauthTokenFromRequest = oauthToken.substr( found + 12, endToken - 12 );
    }
    return 0;
}

// Using oauth token that is setting in function requestToken( ).
// Get pin from user and change it for user oauth token and user ouath secret
int
Twitter::requestAccessToken( )
{
    std::cout << "Please copy that link below and paste in browser\n";
    std::string pin;
    std::cout << ">>>> "
              << "https://api.twitter.com/oauth/authorize?oauth_token=" + m_oauthTokenFromRequest
              << " <<<<\n";
    std::cout << "Then back to the consol and enter the PIN: ";
    std::getline( std::cin, pin );

    std::string tmpURL = "api.twitter.com/oauth/access_token";
    m_signature.clear( );
    std::time_t t = std::time( 0 );
    m_signingKey = m_clientSecret + "&" + m_oauthTokenSecret;

    m_signature[VERIFIER] = pin;
    m_signature[CALLBACK] = "oob";
    m_signature[TOKEN] = m_oauthTokenFromRequest;
    m_signature[CONSUMERKEY] = m_clientKey;
    m_signature[NONCE] = generateToken( );
    m_signature[SIGNATUREMETHOD] = "HMAC-SHA1";
    m_signature[TIMESTAMP] = std::to_string( t );
    m_signature[VERSION] = "1.0";
    m_signature[SIGNATURE] = createSignature( tmpURL );

    m_signature.erase( VERIFIER );
    std::string body = VERIFIER + "=" + pin;
    m_httpClient->httpsEnable( m_certPath );
    m_httpClient->addHeader( AUTHORIZATION, authorizationHeader( ) );
    m_httpClient->sendPost( tmpURL, body );
    rapidjson::Document doc;
    doc.Parse( m_httpClient->getData( ).c_str( ) );
    if ( doc.HasMember( "errors" ) )
    {
        std::cout << m_httpClient->getData( );
        return BAD_AUTHENTICATION;
    }

    std::string oauthToken = m_httpClient->getData( );
    size_t found = oauthToken.find( "oauth_token=" );
    size_t endToken = oauthToken.find( "&", found );
    // std::cout << oauthToken << std::endl;
    if ( found != std::string::npos )
    {
        m_userToken = oauthToken.substr( found + 12, endToken - 12 );
    }
    found = oauthToken.find( "oauth_token_secret=" );
    endToken = oauthToken.find( "&", endToken );
    if ( found != std::string::npos )
    {
        m_userOauthTokenSecret = oauthToken.substr( found + 19, endToken - 17 );
    }
    m_signature.clear( );
    return 0;
}

std::string
Twitter::createSignature( const std::string& tmpURL )
{
    int size = m_signature.size( );
    std::string baseString;
    for ( auto& it : m_signature )
    {
        --size;
        baseString += encodePercent( it.first ) + "=" + encodePercent( it.second );
        if ( 0 == size )
        {
            break;
        }
        baseString += "&";
    }

    std::string baseSignature = "POST&";
    baseSignature += encodePercent( "https://" + tmpURL ) + "&" + encodePercent( baseString );
    // std::cout << "\n" << baseSignature << "\n" << std::endl;

    std::vector< unsigned char > signKey( m_signingKey.size( ) );
    for ( unsigned int i = 0; i < m_signingKey.size( ); i++ )
    {
        signKey[i] = m_signingKey[i];
    }
    std::vector< unsigned char > baseSignatureVector( baseSignature.size( ) );
    for ( unsigned int i = 0; i < baseSignature.size( ); i++ )
    {
        baseSignatureVector[i] = baseSignature[i];
    }
    unsigned char buffer[64] = {0};
    size_t len = sizeof( buffer );
    hmac_sha1( signKey.data( ), m_signingKey.size( ), baseSignatureVector.data( ),
               baseSignature.size( ), buffer, &len );

    std::vector< unsigned char > toEncode;
    int i = 0;
    for ( auto it : buffer )
    {
        if ( 0 == it )
        {
            break;
        }
        toEncode.push_back( it );
        ++i;
    }
    return encodeBase64( toEncode );
}

std::string
Twitter::authorizationHeader( )
{
    int size = m_signature.size( );
    std::string baseString = "OAuth ";
    for ( auto& it : m_signature )
    {
        --size;
        baseString += encodePercent( it.first );
        baseString += "=\"";
        baseString += encodePercent( it.second );
        baseString += "\"";
        if ( 0 == size )
        {
            continue;
        }
        baseString += ", ";
    }
    return baseString;
}

std::string
Twitter::generateToken( )
{
    const std::string TAB = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device r;
    std::default_random_engine e1( r( ) );
    std::uniform_int_distribution< int > dist( 0, 61 );
    std::string returnValue;
    for ( int i = 5; i > 0; --i )
    {
        returnValue += TAB[dist( e1 )];
    }
    return returnValue;
}

std::string
Twitter::authAppOnly( )
{
    std::string authorizationHeader = m_clientKey;  //"kfXTHiQSY6RcqyjajepWkMU0a";
    authorizationHeader.push_back( ':' );
    authorizationHeader.append(
        m_clientSecret );  //"6WWgPTkYUrDwGMijel9bKzkYl8UjUaBtCRYo43OOZgBEiOymkW"
    std::vector< unsigned char > toEncode;
    for ( auto it : authorizationHeader )
    {
        toEncode.push_back( it );
    }
    authorizationHeader = "Basic ";
    authorizationHeader += encodeBase64( toEncode );
    m_httpClient->httpsEnable( m_certPath );
    m_httpClient->addHeader( USERAGENT, "AppConnectWithTwitter" );
    m_httpClient->addHeader( AUTHORIZATION, authorizationHeader );
    m_httpClient->addHeader( CONTENTYPE, "application/x-www-form-urlencoded;charset=UTF-8" );
    m_httpClient->sendPost( "api.twitter.com/oauth2/token", "grant_type=client_credentials" );
    std::string bearer = "Bearer ";
    rapidjson::Document doc;
    doc.Parse( m_httpClient->getData( ).c_str( ) );
    return bearer += doc["access_token"].GetString( );
}
