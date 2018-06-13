#include "encodeFunctions.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

std::string
encodeBase64( std::vector< unsigned char >& toEncode )
{
    const std::string TAB = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string returnString;
    unsigned int set8Bits = 0;
    int moveBits = -6;
    for ( unsigned char c : toEncode )
    {
        set8Bits = ( set8Bits << 8 ) + c;
        moveBits += 8;
        while ( moveBits >= 0 )
        {
            returnString.push_back( TAB[( set8Bits >> moveBits ) & 0x3fu] );
            moveBits -= 6;
        }
    }
    if ( moveBits > -6 )
    {
        returnString.push_back( TAB[( ( set8Bits << 8 ) >> ( moveBits + 8 ) ) & 0x3fu] );
    }
    while ( returnString.size( ) % 4 )
    {
        returnString.push_back( '=' );
    }
    return returnString;
}

std::string
encodeBase64v2( const std::string& input )
{
    const std::string TAB = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string strReturn;
    unsigned int i = 0;
    while ( i < input.size( ) )
    {
        int set8Bits = 0;
        for ( int j = 0; j < 3; ++j )
        {
            if ( 0 == ( input.size( ) - i ) )
            {
                set8Bits = ( set8Bits << 8 ) + input[i];
                break;
            }
            else
            {
                set8Bits = ( set8Bits << 8 ) + input[i];
                ++i;
            }
        }
        for ( int k = 18; k >= 0; k -= 6 )
        {
            if ( ( 0 == input.size( ) - i ) && ( 0 != ( input.size( ) % 3 ) ) )
            {
                int j = 0;
                ( 2 == input.size( ) % 3 ) ? j = 18 : j = 10;
                for ( ; j > 0; j -= 6 )
                {
                    strReturn.push_back( TAB[( set8Bits >> j ) & 0x3f] );
                }
                break;
            }
            strReturn.push_back( TAB[( set8Bits >> k ) & 0x3f] );
        }
    }
    while ( strReturn.size( ) % 4 )
    {
        strReturn.push_back( '=' );
    }
    return strReturn;
}

std::string
decodeBase64( const std::string& str )
{
    const std::string TAB = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string returnString;
    std::vector< int > T( 128, -1 );
    for ( int i = 0; i < 64; i++ )
    {
        T[TAB[i]] = i;
    }
    unsigned int i = 0;
    while ( i < str.size( ) )
    {
        int value = 0;
        int j = 0;
        for ( ; j < 4; j++ )
        {
            if ( -1 == T[str[i]] )
            {
                ++i;
                break;
            }
            else
            {
                value = ( value << 6 ) + T[str[i]];
                ++i;
            }
        }
        for ( int k = 16; k >= 0; k -= 8 )
        {
            if ( str.size( ) - i < 4 )
            {
                if ( 3 == j )
                {
                    returnString.push_back( char( ( value >> 10 ) & 0xff ) );
                    returnString.push_back( char( ( value >> 2 ) & 0xff ) );
                    break;
                }
                else if ( 2 == j )
                {
                    returnString.push_back( char( ( value >> 4 ) & 0xff ) );
                    break;
                }
            }
            returnString.push_back( char( ( value >> k ) & 0xff ) );
        }
    }
    return returnString;
}

std::string
encodePercent( const std::string& str )
{
    const std::string TAB = "!@#$/=+&,%[]:;'?()* ";
    std::string returnString = "";
    for ( char c : str )
    {
        size_t found = TAB.find( c );
        if ( found != std::string::npos )
        {
            std::stringstream stream;
            stream << std::uppercase << std::hex << static_cast< int >( c );
            std::string percentCode( stream.str( ) );
            percentCode = "%" + percentCode;
            returnString.append( percentCode );
        }
        else
        {
            returnString.push_back( c );
        }
    }
    return returnString;
}

std::string
decodePercent( const std::string& str )
{
    std::string returnString = "";
    for ( unsigned int i = 0; i <= str.size( ); )
    {
        if ( '%' == str[i] )
        {
            std::string temp = "0x";
            temp.push_back( str[i + 1] );
            temp.push_back( str[i + 2] );
            int value = stoi( temp, nullptr, 0 );
            returnString.push_back( static_cast< char >( value ) );
            i += 3;
        }
        else
        {
            returnString.push_back( str[i] );
            i++;
        }
    }
    return returnString;
}

std::string
generateToken( )
{
    const std::string TAB = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device r;
    std::default_random_engine e1( r( ) );
    std::uniform_int_distribution< int > dist( 0, 63 );
    std::string returnValue;
    for ( int i = 32; i > 0; --i )
    {
        returnValue += TAB[dist( e1 )];
    }
    return returnValue;
}

std::string
createSignature( const std::map< std::string, std::string >& keys )
{
    std::string strResult;
    unsigned int increment = 0;
    for ( auto& i : keys )
    {
        ++increment;
        strResult += encodePercent( i.first );
        strResult += "=";
        strResult += encodePercent( i.second );
        if ( increment == keys.size( ) )
        {
            return strResult;
        }
        strResult += "&";
    }
    return strResult;
}

std::string
createAuthentication( const std::map< std::string, std::string >& keys )
{
    unsigned int increment = 0;
    std::string strResult;
    for ( auto& i : keys )
    {
        ++increment;
        strResult += encodePercent( i.first );
        strResult += "=\"";
        strResult += encodePercent( i.second );
        strResult += "\"";
        if ( increment == keys.size( ) )
        {
            return strResult;
        }
        strResult += ", ";
    }
    return strResult;
}
