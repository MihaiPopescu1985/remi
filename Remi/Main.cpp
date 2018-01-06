#include <Windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

#include <SFML/Graphics.hpp>

#include "buton.h"
#include "server.h"
#include "client.h"
#include "enum.cpp"

bool conectare( sf::TcpSocket & socket );

#pragma warning(suppress: 28251)
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow )
{
	srand( unsigned int( time( NULL )));
	sf::Uint32 q = rand( ) % 50000 ;

	server server ;
	client client ;
	sf::Packet pachet ;
	etapa_initiala e_initiala = prima_pagina ;
	std::string chr ;
	std::string str ;
	bool key_pressed = false ;

	sf::Font font ;
	font.loadFromFile( "Sansation.ttf" );
	sf::Text text ;
	text.setFont( font );
	text.setCharacterSize( 20 );
	text.setPosition( 148, 30 );
	text.setColor( sf::Color::White );

	sf::Text text_prima_pagina ;
	text_prima_pagina.setFont( font );
	text_prima_pagina.setCharacterSize( 15 );
	text_prima_pagina.setColor( sf::Color::White );

	sf::Text text_server ;
	text_server.setFont( font );
	text_server.setCharacterSize( 15 );
	text_server.setColor( sf::Color::White );

	sf::Text text_client ;
	text_client.setFont( font );
	text_client.setCharacterSize( 15 );
	text_client.setColor( sf::Color::White );

	buton buton_server( "poze/ButonServer.png", sf::Vector2f( 50, 100 ));
	buton buton_client( "poze/ButonClient.png", sf::Vector2f( 250, 100 ));
	buton buton_start( "poze/ButonStart.png", sf::Vector2f( 148.5, 76.5 ));

	sf::RenderWindow window( sf::VideoMode( 450, 200 ), "Remi pe masa - VERSIUNE DE TEST" );

	while( window.isOpen( ))
	{
		sf::Event event ;
		while( window.pollEvent( event ))
		{
			if( event.type == sf::Event::Closed )
			{
				window.close( );
			}
			if( event.type == sf::Event::TextEntered )
			{
				chr = event.text.unicode ;
				key_pressed = true ;
			}
		}
		switch ( e_initiala )
		{
		case( prima_pagina ) :
			text_prima_pagina.setString( "https://www.facebook.com/joculremi" );
			if( buton_client.include_mouse( window ))
			{
				e_initiala = etapa_client ;
				break ;
			}
			if( buton_server.include_mouse( window ))
			{
				e_initiala = etapa_server ;
				break ;
			}
			break ;
		case( etapa_server ) :
			text_server.setString( "Trimite acest numar si apasa START" );
			str = std::to_string( q );

			if( buton_start.include_mouse( window ))
			{
				if( conectare( server.sochet ))
				{
					pachet << str ;
					if( server.sochet.send( pachet ) == sf::Socket::Done )
					{
						server.run( );
						window.close( );
						break ;
					}
				}
			}
			break ;
		case( etapa_client ) :
			text_client.setString( "Tasteaza numarul primit si apasa START" );
			if( key_pressed == true )
			{
				if( sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace ))
				{
					if( str.length( ) > 0 )
					{
						str.pop_back( );
						key_pressed = false ;
						break ;
					}
				}
				else
				{
					str += chr ;
					key_pressed = false ;
					break ;
				}
			}
			if( buton_start.include_mouse( window ))
			{
				if( conectare( client.socket_client ))
				{
					pachet << str ;
					if( client.socket_client.send( pachet ) == sf::Socket::Done )
					{
						client.run( );
						window.close( );
						break ;
					}
				}
			}
			break ;
		}
		text.setString( str );
		window.clear( );

		switch ( e_initiala )
		{
		case( prima_pagina ) :
			window.draw( buton_server.draw_buton( ));
			window.draw( buton_client.draw_buton( ));
			window.draw( text_prima_pagina );
			break ;
		case( etapa_client ) :
			window.draw( buton_start.draw_buton( ));
			window.draw( text_client );
			window.draw( text );
			break ;
		case( etapa_server ) :
			window.draw( buton_start.draw_buton( ));
			window.draw( text_server );
			window.draw( text );
			break ;
		}
		window.display( );
	}
	return 0 ;
}
bool conectare( sf::TcpSocket & socket )
{
	std::string IP ;
	char input_line ;
	std::ifstream file_in( "IP.txt" );

	while( file_in.get( input_line ) && !file_in.eof( ))
	{
		IP += input_line ;
	}
	file_in.close( );

	if( socket.connect( IP , 31111 ) == sf::Socket::Done )
	{
		return true ;
	}
	return false ;
}