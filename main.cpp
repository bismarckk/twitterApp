#include "Socket/httpClient.h"
#include "TwitsContainer/twittsContainer.h"
#include "twitterClient.h"
#include <iostream>

int
main( )
{
    Twitter obj;
    HttpClient client;
    obj.setHttpClient( &client );
    obj.setCertPath(
        "/home/sypniewski/Documents/Projekt/TwitterApp/TwitterApp/Cert/"
        "DigiCertHighAssuranceEVRootCA.crt" );
    obj.setClientKeyAndSecret( "d8TGplNTFRTJ32N9BSekNBOp7",
                               "djGRN2S2pYcrq0ifRQjEipfaaHWUyLiULhBHDmIoAPtHNwQJ22" );
    obj.setOauthTokenAndSecret( "1001440142081568769-5ABB9Mt79pmhc3j20qfsNHPMtOvLvC",
                                "wqaZ9ffYmnsGe6zEEUO8d81gYqjUDSesVA5GrWF6DSfYd" );
    TwittsContainer container = obj.getTwitts( 2, "Polsport" );
    std::cout << container.getSize( );
    for ( size_t i = 0; i < container.getSize( ); i++ )
    {
        std::cout << container.getTwitt( i ) << std::endl;
    }
    std::cout << container[1].getText( );
    while ( true )
    {
        std::string tmpStr;
        std::cout << "\nWpisz tekst: ";
        std::getline( std::cin, tmpStr );
        obj.sendTwitt( tmpStr );
    }
    return 0;
    /////asldalsd
}
