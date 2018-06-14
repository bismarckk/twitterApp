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
    Twitt( const std::string& author, const std::string& date, const std::string& text );
    std::string getAuthor( ) const;
    std::string getDate( ) const;
    std::string getText( ) const;

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
    const Twitt& getTwitt( int selectedTwitt ) const;
    const Twitt& operator[]( size_t selected ) const;

private:
    rapidjson::Document m_doc;
    std::vector< Twitt > m_twittVector;
    void fillVector( );
};

#endif
