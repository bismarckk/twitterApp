#include "httpClient.h"

#include <algorithm>
#include <iostream>

HttpClient::HttpClient( ) : m_easyCurl( curl_easy_init( ) ), m_list( NULL ), m_http( "http://" ) {}
void
HttpClient::sendGet( const std::string& url )
{
    m_data.clear( );
    std::string tmpURL = m_http + url;
    curl_easy_setopt( m_easyCurl, CURLOPT_HTTPGET, 1 );
    curl_easy_setopt( m_easyCurl, CURLOPT_URL, tmpURL.c_str( ) );
    sendToServer( );
}

void
HttpClient::addHeader( const std::string& key, const std::string& value )
{
    std::string str = key + ": " + value;
    m_list = curl_slist_append( m_list, str.c_str( ) );
}

void
HttpClient::sendPost( const std::string& url, const std::string& body )
{
    std::string tmpURL = m_http + url;
    curl_easy_setopt( m_easyCurl, CURLOPT_URL, tmpURL.c_str( ) );

    curl_easy_setopt( m_easyCurl, CURLOPT_POSTFIELDSIZE, static_cast< long >( body.size( ) ) );
    curl_easy_setopt( m_easyCurl, CURLOPT_POSTFIELDS, body.c_str( ) );

    sendToServer( );
}

void
HttpClient::sendToServer( )
{
    // curl_easy_setopt( m_easyCurl, CURLOPT_VERBOSE, 1 );
    m_data.clear( );
    m_header.clear( );
    curl_easy_setopt( m_easyCurl, CURLOPT_WRITEFUNCTION, &HttpClient::writeCallbackData );
    curl_easy_setopt( m_easyCurl, CURLOPT_WRITEDATA, &m_data );
    curl_easy_setopt( m_easyCurl, CURLOPT_HEADERFUNCTION, &HttpClient::writeCallbackHeader );
    curl_easy_setopt( m_easyCurl, CURLOPT_HEADERDATA, &m_header );
    curl_easy_setopt( m_easyCurl, CURLOPT_HTTPHEADER, m_list );
    curl_easy_perform( m_easyCurl );
    curl_slist_free_all( m_list );
    m_list = NULL;
}

const std::string&
HttpClient::getData( ) const
{
    return m_data;
}

const std::string&
HttpClient::getHeader( ) const
{
    return m_header;
}

size_t
HttpClient::writeCallbackData( char* ptr, size_t size, size_t nmemb, void* data )
{
    std::string* str = static_cast< std::string* >( data );
    size_t newLength = size * nmemb;
    std::copy( (char*)ptr, (char*)ptr + newLength, std::back_inserter( *str ) );
    return size * nmemb;
}

size_t
HttpClient::writeCallbackHeader( char* ptr, size_t size, size_t nmemb, void* header )
{
    std::string* str = static_cast< std::string* >( header );
    size_t newLength = size * nmemb;
    std::copy( (char*)ptr, (char*)ptr + newLength, std::back_inserter( *str ) );
    return size * nmemb;
}

void
HttpClient::httpsEnable( const std::string& certPath )
{
    // std::string cert = certPath;
    // std::cout << cert;
    // "/home/sypniewski/Documents/Projekt/TwitterApp/TwitterApp/Cert/DigiCertHighAssuranceEVRootCA.crt"
    m_http = "https://";
    curl_easy_setopt( m_easyCurl, CURLOPT_CAINFO, certPath.c_str( ) );
    curl_easy_setopt( m_easyCurl, CURLOPT_SSL_VERIFYHOST, 1L );
}
void
HttpClient::httpsDisable( )
{
    m_http = "http://";
    curl_easy_setopt( m_easyCurl, CURLOPT_SSL_VERIFYHOST, 0L );
}
