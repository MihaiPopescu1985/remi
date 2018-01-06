#ifndef SERVER_H
#define SERVER_H

#include <time.h>
#include <string>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Carti.h"
#include "jucator.h"

class server
{
private :
	bool cheat ;
	sf::Int32 CARTI_DECARTATE ;
	jucator jucator ;
	etapa_joc etapa ;

	sf::Uint32 int_in, int_out ;
	std::string string_in, string_out ;
	sf::Packet pachet_in, pachet_out ;
	std::list <sf::Packet> pack ;/////////////////////////////////////////////////////////////

	sf::Uint32 nr_carte, x_carte, y_carte ; // necesare pentru inchiderea jocului
public :
	sf::TcpSocket sochet ;

public :
	void run( );
	server( );
};
server::server( )
{
	CARTI_DECARTATE = 0 ;
	etapa = decartez ;
	
}
void server::run( )
{
	sf::RenderWindow window( sf::VideoMode( 1350, 680 ), "Remi pe tabla - server", 8 );

	cheat = false ;
	srand(( unsigned int )time( NULL )) ;

// INITIALIZEAZA PUNCTELE DE PE MASA

	sf::Vector2f puncte_masa[106] ;

	for( int k = 0; k < 15; k++ )
	{
		for( int kk = 0; kk < 7; kk++ )
		{
			puncte_masa[k*7+kk].x = ( float ) ( 70 * k ) + 50 ;
			puncte_masa[k*7+kk].y = ( float ) 200 + 2 * ( kk );
		}
	}
	puncte_masa[105].x = 1100.f ;
	puncte_masa[105].y = 212.f ;

// AMESTECARE PUNCTE MASA

	sf::Vector2f swap_pc_masa ;

	for( int i = 0; i < 106; i++ )
	{
		int q = rand() % 106 ;
		if( q != i )
		{
			swap_pc_masa = puncte_masa[i] ;
			puncte_masa[i] = puncte_masa[q] ;
			puncte_masa[q] = swap_pc_masa ;
		}
	}
// INITIALIZEAZA PUNCTELE DE PE TABLA

	sf::Vector2f puncte_tabla[30] ;

	for( int i = 0; i < 15; i++ )
	{
		puncte_tabla[i].x = (float) 70 * i + 100 ;
		puncte_tabla[i].y = (float) 500 ;

		puncte_tabla[i+15].x = (float) 70 * i + 100 ;
		puncte_tabla[i+15].y = (float) 573 ;
	}
// INITIALIZEAZA MATRICEA CU CARTI

	Carti carte[106];

	for( int i = 0; i < 106; i++ )
	{
		carte[i].n = i ;
		carte[i].sprite_carte.setPosition( puncte_masa[i] );
	}
// ARATA ULTIMA CARTE DIN PACHET

	for( int i = 0; i < 106; i++ )
	{
		if( carte[i].sprite_carte.getPosition().x == 1100.f &&
			carte[i].sprite_carte.getPosition().y == 212.f )
		{
			carte[i].set_carte( );
			carte[i].etapa_carte = atu ;

			// trimite ultima carte

			int_out = carte[i].n ;
			string_out = "atu" ;
			pachet_out << int_out << string_out ;

			pack.push_back( pachet_out );
			//sochet.send( pachet_out );
			pachet_out.clear( );
		}
	}
// PUNE CARTILE PE TABLA

	while( CARTI_DECARTATE < 15 )
	{
		int k = CARTI_DECARTATE / 7 ;
		int kk = CARTI_DECARTATE % 7 ;

		for( int i = 0; i < 106; i++ )
		{
			if( carte[i].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
				carte[i].sprite_carte.getPosition().y == ( float ) 200 + 2 * ( kk ))
			{
				carte[i].set_carte( );
				carte[i].sprite_carte.setPosition( puncte_tabla[CARTI_DECARTATE] );
				carte[i].etapa_carte = pe_tabla ;
				carte[i].pozitie_corecta = true ;
			}
		}
		CARTI_DECARTATE++ ;
	}
// INITIALIZEAZA MATRICEA CU RECTANGULI NECESARE PENTRU TABLA

	sf::RectangleShape rectangles[30] ;

	for( int a = 0; a < 30; a++ )
	{
		rectangles[a].setSize( sf::Vector2f( 70, 72 ));
		rectangles[a].setPosition( puncte_tabla[a].x, puncte_tabla[a].y );
	}
// TRIMITE CARTILE PARTENERULUI

	for( int i = 0; i < 14; i++ )
	{
		int k = CARTI_DECARTATE / 7 ;
		int kk = CARTI_DECARTATE % 7 ;

		for( int i = 0; i < 106; i++ )
		{
			if( carte[i].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
				carte[i].sprite_carte.getPosition().y == ( float ) 200 + 2 * ( kk ) &&
				carte[i].etapa_carte == pe_masa )
			{
				carte[i].etapa_carte = anulata ;
				carte[i].sprite_carte.setPosition( 0, 0 );
				int_out = carte[i].n ;
				string_out = "pe tabla" ;
				pachet_out << int_out << string_out ;
				
				pack.push_back( pachet_out );
				pachet_out.clear( );
			}
		}
		CARTI_DECARTATE++ ;
	}
	// afiseaza un text
		sf::Text text ;
		sf::Font font ;
		font.loadFromFile( "Sansation.ttf" );
		text.setFont( font );
		text.setCharacterSize( 20 );
		text.setColor( sf::Color::White );
		text.setPosition( 400, 400 );//////////////////////////////// Nu trebuie lasat asa //////////////////
		text.setString( "Decarteaza !" );

		sochet.setBlocking( false );
// LOOP JOC
	while (window.isOpen( ))
	{
		sf::Event event ;
		window.setPosition( sf::Vector2i( 0, 0 ));
		window.setFramerateLimit( 30 );

		while ( window.pollEvent( event ))
		{
			if( event.type == sf::Event::Closed )
			{
				window.close( );
			}
			if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )
			{
				window.close( );
			}
		}
		// pozitia mouseului
		sf::Vector2f mouse_pos ;

		mouse_pos.x = ( float ) sf::Mouse::getPosition( window ).x ;
		mouse_pos.y = ( float ) sf::Mouse::getPosition( window ).y ;

		// drag carte
		for( int i = 0; i < 106; i++ )
		{
			carte[i].drag_carte( window );

		// Cartile stau decat pe tabla
			for( int j = 0; j < 30; j++ )
			{
				if( rectangles[j].getGlobalBounds().contains( carte[i].sprite_carte.getPosition().x
					+ 34, carte[i].sprite_carte.getPosition().y + 34  ) &&
					!sf::Mouse::isButtonPressed( sf::Mouse::Left ))
				{
					carte[i].sprite_carte.setPosition( rectangles[j].getPosition( ));
					carte[i].pozitie_corecta = true ;
				}
			}
			if( carte[i].pozitie_corecta == false )
			{
				carte[i].sprite_carte.setPosition( carte[i].pozitie_curenta );
				carte[i].pozitie_corecta = true ;
			}
		}
		// cartile nu se pot suprapune
		for( int i = 0; i < 106; i++ )
		{
			if( carte[i].etapa_carte == pe_tabla )
			{
				for( int j = 0; j < 106; j++ )
				{
					if( carte[j].etapa_carte == pe_tabla )
					{
						if( i != j && carte[i].sprite_carte.getPosition( ) == carte[j].sprite_carte.getPosition( ))
						{
							carte[j].sprite_carte.setPosition( carte[j].pozitie_curenta );
						}
					}
				}
			}
		}
		// primeste carte
		if( etapa != final && etapa != prefinal && sochet.receive( pachet_in ) == sf::Socket::Done )
		{
			pachet_in >> int_in >> string_in ;
			pachet_in.clear( );
		}
		// trisare
		if( cheat == false )
		{
			int k = CARTI_DECARTATE / 7 ;
			int kk = CARTI_DECARTATE % 7 ;

			for( int i = 0; i < 106; i++ )
			{
				if( carte[i].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
					carte[i].sprite_carte.getPosition().y == ( float ) 200 + 2 * ( kk ) &&
					carte[i].etapa_carte == pe_masa )
				{
					if( carte[i].sprite_carte.getGlobalBounds( ).contains( mouse_pos ) &&
						sf::Mouse::isButtonPressed( sf::Mouse::Right ))
					{
						carte[i].set_carte( );
					}
					if( ! carte[i].sprite_carte.getGlobalBounds( ).contains( mouse_pos ) &&
						carte[i].imagine_carte != "poze/blank.png" )
					{
						carte[i].imagine_carte = "poze/blank.png" ;
						carte[i].textura_carte.loadFromFile( carte[i].imagine_carte );
						carte[i].sprite_carte.setTexture( carte[i].textura_carte );
						cheat = true ;
					}
				}
			}
		}
		if( string_in == "am terminat" )
		{
			etapa = prefinal ;

			if( sochet.receive( pachet_in ) == sf::Socket::Done )
			{
				pachet_in >> nr_carte >> x_carte >> y_carte ;
				pachet_in.clear( );

				carte[nr_carte].sprite_carte.setPosition( (float)x_carte, (float)( y_carte - 500 ));
				carte[nr_carte].etapa_carte = decartata ;
				carte[nr_carte].set_carte( );
			}
		}
		if( etapa == decartez )
		{
			int k = CARTI_DECARTATE / 7 ;
			int kk = CARTI_DECARTATE % 7 ;

			for( int i = 0; i < 106; i++ )
			{
				if( carte[i].etapa_carte == is_dragging &&
					jucator.sprite_decartare.getGlobalBounds().contains( mouse_pos ) &&
					!sf::Mouse::isButtonPressed( sf::Mouse::Left ))
				{
					// decartez si trimit cartea decartata
					carte[i].sprite_carte.setPosition( 1087, 400 );
					carte[i].etapa_carte = anulata ;
					int_out = carte[i].n ;
					string_out = "carte decartata";
					pachet_out << int_out << string_out ;
					
					pack.push_back( pachet_out );
					pachet_out.clear( );
					
					for( int l = 0; l < 106; l++ )
					{
						if( carte[l].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
							carte[l].sprite_carte.getPosition().y == ( float ) 200 + 2 * ( kk ))
						{
							// trimite cartea de pe masa
							int_out = carte[l].n ;
							string_out = "de pe masa" ;
							pachet_out << int_out << string_out ;
							
							pack.push_back( pachet_out );
							pachet_out.clear( );
							text.setString( "Asteptam carte." );
							etapa = primesc_carte ;
							break ;
						}
					}
				}
			}
		}
		if( string_in == "am luat de pe masa" )
		{
			for( int i = 0; i < 106; i++ )
			{
				if( carte[i].n == int_in )
				{
					carte[i].etapa_carte = anulata ;
					CARTI_DECARTATE++ ;
					string_in = "ok" ;
					break ;
				}
			}
		}
		// arata cartea primita
		if( string_in == "carte decartata" )
		{
			carte[int_in].sprite_carte.setPosition( 100, 400 );
			for( int j = 0; j < 106; j++ )
			{
				if( carte[j].etapa_carte == decartata )
				{
					carte[j].etapa_carte = anulata ;
				}
			}
			carte[int_in].etapa_carte = decartata ;
			carte[int_in].set_carte( );
			text.setString( "Alege cartea decartata sau de pe masa !" );
			etapa = aleg_carte ;
			string_in = "ok" ;
		}
		// alege carte de pe masa sau cartea decartata
		if( etapa == aleg_carte )
		{
			int k = CARTI_DECARTATE / 7 ;
			int kk = CARTI_DECARTATE % 7 ;

			for( int i = 0; i < 106; i++)
			{
				if( carte[i].sprite_carte.getGlobalBounds().contains( mouse_pos ) &&
					sf::Mouse::isButtonPressed( sf::Mouse::Left ))
				{
					// daca se alege cartea decartata de catre partener
					if( carte[i].sprite_carte.getPosition( ).x == 100 &&
						carte[i].sprite_carte.getPosition( ).y == 400 &&
						carte[i].etapa_carte == decartata )
					{
						carte[i].etapa_carte = pe_tabla ;
						text.setString( "Decarteaza" );
						etapa = decartez ;
						break ;
					}
					// daca aleg cartea de pe masa
					if( carte[i].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
						carte[i].sprite_carte.getPosition().y == ( float ) 200 + 2 * ( kk ))
					{
						carte[i].set_carte( );
						carte[i].etapa_carte = pe_tabla ;
						CARTI_DECARTATE++ ;
						int_out = -1 ;
						string_out = "am luat de pe masa";
						pachet_out << int_out << string_out ;
						
						pack.push_back( pachet_out );
						pachet_out.clear( );
						text.setString( "Decarteaza" );
						etapa = decartez ;
						break ;
					}
				}
			}
		}
		if( jucator.inchide.include_mouse( window ))
		{
			text.setString( "Ai inchis !" );
			int_out = 0 ;
			string_out = "am terminat" ;
			pachet_out << int_out << string_out ;
			
			pack.push_back( pachet_out );
			pachet_out.clear( );

			etapa = final ;

			for( int i = 0; i < 105; i++ )
			{
				if( carte[i].etapa_carte == pe_tabla )
				{
					nr_carte = carte[i].n ;
					x_carte = (sf::Uint32)carte[i].sprite_carte.getPosition().x;
					y_carte = (sf::Uint32)carte[i].sprite_carte.getPosition().y;

					pachet_out << nr_carte << x_carte << y_carte ;
					
					pack.push_back( pachet_out );
					pachet_out.clear( );
				}
			}
		}
		if( pack.size( ) > 0 )
		{
			if( sochet.send( pack.front( )) == sf::Socket::Done )
			{
				pack.pop_front( );
			}
		}

// DESENARE
		window.clear( );
		
		jucator.desenare( window );

		for( int i = 0; i < 106; i++ )
		{
			int k = i / 7 ;
			int kk = i % 7 ;

			for( int j = 0; j < 106; j++ )
			{
				if( carte[j].sprite_carte.getPosition().x == ( float ) ( 70 * k ) + 50 &&
					carte[j].sprite_carte.getPosition().y == ( float ) 212 - 2 * ( kk ) &&
					carte[j].etapa_carte == pe_masa )
				{
					window.draw( carte[j].sprite_carte );
				}
				if( carte[j].sprite_carte.getPosition().x == 1100.f &&
					carte[j].sprite_carte.getPosition().y == 212.f )
				{
					window.draw( carte[j].sprite_carte );
				}
			}
			if( carte[105-i].etapa_carte == pe_tabla ||
				carte[105-i].etapa_carte == decartata )
			{
				window.draw( carte[105-i].sprite_carte );
			}
		}
		for( int i = 0; i < 106; i++ )
		{
			if( carte[105-i].etapa_carte == is_dragging )
			{
				window.draw( carte[105-i].sprite_carte );
			}
		}
		window.draw(text);
		window.display( );
	}
	return ;
}
#endif