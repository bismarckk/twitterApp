#ifndef TWITTSCONTAINER_H
#define TWITTSCONTAINER_H

#include "rapidjson/include/rapidjson/document.h"

#include <string>
#include <vector>

////////////////////////TWITT///////////////////////////
class Twitt
{
public:
    Twitt( );
    Twitt( std::string author, std::string date, std::string text );
    std::string getAuthor( ) const;
    std::string getDate( ) const;
    std::string getText( ) const;
    std::string operator=( const Twitt& rhs );

private:
    std::string m_author;
    std::string m_date;
    std::string m_text;
};

std::ostream& operator<<( std::ostream& os, const Twitt& rhs );

////////////////////TWITTSCONTAINER////////////////////
class TwittsContainer : private Twitt
{
public:
    TwittsContainer( );
    TwittsContainer( const std::string& jsonDocument );
    size_t getSize( ) const;
    Twitt getTwitt( int selectedTwitt );
    Twitt operator[]( size_t selected );

private:
    rapidjson::Document m_doc;
    std::vector< Twitt > m_twittVector;
    void fillVector( );
};

#endif
