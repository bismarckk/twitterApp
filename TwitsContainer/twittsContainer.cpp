#include "twittsContainer.h"

#include <iostream>

////////////////////////CLASS TWITT///////////////////////////
Twitt::Twitt( ) {}
Twitt::Twitt( const std::string& author, const std::string& date, const std::string& text )
    : m_author( author ), m_date( date ), m_text( text )
{
}

std::string
Twitt::getAuthor( ) const
{
    return m_author;
}
std::string
Twitt::getDate( ) const
{
    return m_date;
}
std::string
Twitt::getText( ) const
{
    return m_text;
}

std::ostream&
operator<<( std::ostream& os, const Twitt& rhs )
{
    os << rhs.getDate( ) << " " << rhs.getAuthor( ) << " " << rhs.getText( );
    return os;
}

////////////////////CLASS TWITTSCONTAINER////////////////////
TwittsContainer::TwittsContainer( ) {}

TwittsContainer::TwittsContainer( const std::string& jsonDocument )
{
    m_doc.Parse( jsonDocument.c_str( ) );
    fillVector( );
}

const Twitt&
TwittsContainer::getTwitt( int selectedTwitt ) const
{
    return m_twittVector[selectedTwitt];
}

const Twitt& TwittsContainer::operator[]( size_t selected ) const
{
    return m_twittVector[selected];
}

size_t
TwittsContainer::getSize( ) const
{
    return m_twittVector.size( );
}

void
TwittsContainer::fillVector( )
{
    for ( auto& it : m_doc.GetArray( ) )
    {
        m_twittVector.push_back( Twitt( it["user"]["name"].GetString( ),
                                        it["created_at"].GetString( ), it["text"].GetString( ) ) );
    }
}
