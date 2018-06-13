#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <cstddef>
#include <curl/curl.h>
#include <string>

class HttpClient
{
public:
    HttpClient( );
    void sendGet( const std::string& url );
    void addHeader( const std::string& key, const std::string& value );
    void sendPost( const std::string& url, const std::string& body );
    const std::string& getData( ) const;
    const std::string& getHeader( ) const;
    void httpsEnable( const std::string& certPath );
    void httpsDisable( );

private:
    static size_t writeCallbackData( char* ptr, size_t size, size_t nmemb, void* data );
    static size_t writeCallbackHeader( char* ptr, size_t size, size_t nmemb, void* header );
    void sendToServer( );
    CURL* m_easyCurl;
    curl_slist* m_list;
    std::string m_data;
    std::string m_header;
    std::string m_http;
    std::string m_bearerApp;
};
#endif
