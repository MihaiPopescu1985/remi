#ifndef JUCATOR_H
#define JUCATOR_H

#include <SFML/Graphics.hpp>
#include <string>
#include "buton.h"

class jucator
{
private :
	sf::Texture textura_jucator ;
	sf::Texture textura_decartare ;
	sf::Texture text_carte_primita ;
	sf::Texture textura_background ;

	std::string str_buton ;
	sf::Vector2f poz_buton ;
	
public:
	void desenare( sf::RenderWindow & window );
public:
	sf::Sprite sprite_jucator ;
	sf::Sprite sprite_decartare ;
	sf::Sprite sprite_carte_primita ;
	sf::Sprite sprite_background ;

	buton inchide ;

	jucator( );
};
void jucator::desenare( sf::RenderWindow & window )
{
	window.draw( sprite_background );
	window.draw( sprite_jucator );
	window.draw( sprite_decartare );
	window.draw( sprite_carte_primita );
	window.draw( inchide.draw_buton( ));
}
jucator::jucator( )
	: str_buton( "poze/ButonInchide.png" )
	, poz_buton( 1170, 500 )
{
	// arata butonul de inchis

	inchide.set( str_buton, poz_buton );

	// arata background

	textura_background.loadFromFile( "poze/backround.png" );
	sprite_background.setTexture( textura_background );
	sprite_background.setPosition( sf::Vector2f( 0, 0 ));

	// arata tabla

	textura_jucator.loadFromFile( "poze/tabla.png" );
	sprite_jucator.setTexture( textura_jucator );
	sprite_jucator.setPosition( sf::Vector2f ( 100, 500 ));

	// arata unde se decarteaza cartea 

	textura_decartare.loadFromFile( "poze/decartare.png" );
	sprite_decartare.setTexture( textura_decartare );
	sprite_decartare.setPosition( 1087, 400 );

	// arata cartea primita

	text_carte_primita.loadFromFile( "poze/decartare.png" );
	sprite_carte_primita.setTexture( text_carte_primita );
	sprite_carte_primita.setPosition( 100, 400 );
}

#endif // JUCATOR_H