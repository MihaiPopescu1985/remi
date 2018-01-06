#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Graphics.hpp>

#include <string>
#include <list>

#include "Carti.h"
#include "jucator.h"

class client
{
private:
	bool cheat ;
	jucator jucator ;
	etapa_joc etapa ;

	sf::Uint32 int_in, int_out ;
	std::string string_in, string_out ;
	sf::Packet pachet_in, pachet_out ;
	std::list <sf::Packet> pack ;

	sf::Uint32 nr_carte, x_carte, y_carte ; // necesare pentru inchiderea jocului
public :
	sf::TcpSocket socket_client ;
public:
	client( ){};
	void run( );
};
void client::run( )
{	
	sf::RenderWindow window( sf::VideoMode( 1350, 680 ), "Remi pe tabla - client", sf::Style::Fullscreen );

	cheat = false ;

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
		carte[i].sprite_carte.setPosition( puncte_masa[i] );
		carte[i].n = -1 ;
	}
	// se anuleaza primele 15 carti
	for( int i = 0; i < 15; i++ )
	{
		carte[i].etapa_carte = anulata ;
		carte[i].sprite_carte.setPosition( 0, 0 );
	}
// INITIALIZEAZA MATRICEA CU RECTANGULI NECESARE PENTRU TABLA

	sf::RectangleShape rectangles[30] ;

	for( int a = 0; a < 30; a++ )
	{
		rectangles[a].setSize( sf::Vector2f( 70, 72 ));
		rectangles[a].setPosition( puncte_tabla[a].x, puncte_tabla[a].y );
	}
		// afiseaza un text
		sf::Text text ;
		sf::Font font ;
		font.loadFromFile( "Sansation.ttf" );
		text.setFont( font );
		text.setCharacterSize( 20 );
		text.setPosition( 400, 400 );/////////////////////////// nu trebuie lasat asa //////////////////////////////
		text.setString( "Asteptam carte." );
		
		//primeste atu
		if( socket_client.receive( pachet_in ) == sf::Socket::Done )
		{
			pachet_in >> int_in >> string_in ;
			pachet_in.clear( );

			carte[105].n = int_in ;
			carte[105].set_carte( );
			carte[105].sprite_carte.setPosition( 1100, 212 );
		}
		//primeste cartile de pe tabla

		for( int i = 0; i < 14; i++ )
		{
			if( socket_client.receive( pachet_in ) == sf::Socket::Done )
			{
				pachet_in >> int_in >> string_in ;
				pachet_in.clear( );

				carte[i+15].n = int_in ;
				carte[i+15].set_carte( );
				carte[i+15].etapa_carte = pe_tabla ;
				carte[i+15].sprite_carte.setPosition( puncte_tabla[i] );
			}
		}
		socket_client.setBlocking( false );
// LOOP JOC
	while ( window.isOpen( ))
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
		// numai daca se primeste ceva se va deschide pachetul
		if( etapa != final && etapa != prefinal && socket_client.receive( pachet_in ) == sf::Socket::Done )
		{
			pachet_in >> int_in >> string_in ;
			pachet_in.clear( );
		}
		if( string_in == "am terminat" )
		{
			etapa = prefinal ;

			if( socket_client.receive( pachet_in ) == sf::Socket::Done )
			{
				pachet_in >> nr_carte >> x_carte >> y_carte ;
				pachet_in.clear( );

				for( int i = 0; i < 105; i++ )
				{
					if( carte[i].etapa_carte == anulata )
					{
						carte[i].sprite_carte.setPosition( (float)x_carte, (float)( y_carte - 500 ));
						carte[i].n = nr_carte ;
						carte[i].set_carte( );
						carte[i].etapa_carte = decartata ;
						break ;
					}
				}
			}
		}
		// daca serverul ia carte de pe masa, se anuleaza si de catre client
		if( string_in == "am luat de pe masa" )
		{
			for( int i = 0; i < 105; i++ )
			{
				if( carte[i].etapa_carte == pe_masa )
				{
					carte[i].etapa_carte = anulata ;
					string_in = "ok" ;
					break ;
				}
			}
		}
		// arata cartea decartata de catre server
		if( string_in == "carte decartata" )
		{
			for( int i = 0; i < 105; i++ )
			{
				if( carte[i].etapa_carte == anulata && carte[i].n == -1 )
				{
					carte[i].n = int_in ;
					carte[i].sprite_carte.setPosition( 100, 400 );
					carte[i].set_carte( );
					for( int j = 0; j < 105; j++ )
					{
						if( carte[j].etapa_carte == decartata )
						{
							carte[j].etapa_carte = anulata ;
							carte[j].n = -1 ;
						}
					}
					carte[i].etapa_carte = decartata ;
					string_in = "ok" ;
					break ;
				}
			}
		}
		// cartea de pe masa
		if( string_in == "de pe masa" )
		{
			for( int i = 0; i < 105; i++ )
			{
				if( carte[i].etapa_carte == pe_masa && carte[i].n == -1 )
				{
					carte[i].n = int_in ;
					string_in = "ok" ;
					etapa = aleg_carte ;
					text.setString( "Alege cartea decartata sau cea de pe masa " );
					break ;
				}
			}
		}
		// trisare
		if( cheat == false )
		{
			for( int i = 0; i < 105; i++ )
			{
				if( carte[i].etapa_carte == pe_masa && carte[i].n != -1 )
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
		if( etapa == aleg_carte )
		{
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
						// se reseteaza cartea de pe masa
						for( int j = 0; j < 105; j++ )
						{
							if( carte[j].etapa_carte == pe_masa && carte[j].n != -1 )
							{
								carte[j].n = -1 ;
								break ;
							}
						}
						carte[i].etapa_carte = pe_tabla ;
						etapa = decartez ;
						text.setString( "Decarteaza !" );
						break ;
					}
					// daca se alege cartea de pe masa
					if( carte[i].etapa_carte == pe_masa && carte[i].n != -1 )
					{
						carte[i].set_carte( );
						carte[i].etapa_carte = pe_tabla ;
						string_out = "am luat de pe masa" ;
						int_out = carte[i].n ;
						pachet_out << int_out << string_out ;

						pack.push_back( pachet_out ); //////////////////////////////////////////////////////////////////////

						//socket_client.send( pachet_out );
						pachet_out.clear( );
						text.setString( "Decarteaza !" );
						etapa = decartez ;
						break ;
					}
				}
			}
		}
		//trimite carte
		if( etapa == decartez )
		{
			for( int i = 0; i < 106; i++ )
			{
				if( carte[i].etapa_carte == is_dragging &&
					jucator.sprite_decartare.getGlobalBounds().contains( mouse_pos ) &&
					!sf::Mouse::isButtonPressed( sf::Mouse::Left ))
				{
					carte[i].sprite_carte.setPosition( 1087, 400 );
					carte[i].etapa_carte = decartata ;
					int_out = carte[i].n ;
					string_out = "carte decartata";
					pachet_out << int_out << string_out ;

					pack.push_back( pachet_out );

					//socket_client.send( pachet_out );
					pachet_out.clear( );
					text.setString( "Asteptam carte." );
					break ;
				}
			}
		}
		if( jucator.inchide.include_mouse( window ))
		{
			text.setString( "Ai inchis !" );
			string_out = "am terminat" ;
			int_out = 0 ;
			pachet_out << int_out << string_out ;

			pack.push_back( pachet_out );

			//socket_client.send( pachet_out );
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

					//socket_client.send( pachet_out );
					pachet_out.clear( );
				}
			}
		}
		if( pack.size( ) > 0 )
		{
			if( socket_client.send( pack.front( )) == sf::Socket::Done )
			{
				pack.pop_front( );
			}
		}

// DESENARE
		window.clear( );
		
		jucator.desenare( window );

		for( int i = 0; i < 106; i++ )//carti ramase
		{
			int k = i / 7 ;
			int kk = 7 - ( i % 7 );

			if( carte[7*k+kk].etapa_carte == pe_masa )
			{
				window.draw( carte[7*k+kk].sprite_carte );
			}
		}
		for( int i = 0; i < 106; i++ )
		{
			if( carte[i].etapa_carte == pe_tabla || carte[i].etapa_carte == decartata )
				{
					window.draw( carte[i].sprite_carte );
				}
		}
		for( int i = 0; i < 106; i++ )
		{
			if( carte[i].etapa_carte == is_dragging )
			{
				window.draw( carte[i].sprite_carte );
			}
		}
		window.draw( text );
		window.display( );
	}

	return ;
}
#endif
