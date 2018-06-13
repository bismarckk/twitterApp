#ifndef ENCODEFUNCTIONS_H
#define ENCODEFUNCTIONS_H
#include <map>
#include <string>
#include <vector>
std::string encodeBase64( std::vector< unsigned char >& toEncode );
std::string encodeBase64v2( const std::string& str );
std::string encodePercent( const std::string& str );
std::string decodeBase64( const std::string& in );
std::string decodePercent( const std::string& str );
std::string generateToken( );
std::string createSignature( const std::map< std::string, std::string >& v );
std::string createAuthentication( const std::map< std::string, std::string >& v );
#endif
