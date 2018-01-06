#ifndef BUTON_H
#define BUTON_H

#include <SFML/Graphics.hpp>

class buton
{
private :
	bool click ;
	sf::Texture textura_buton ;
	sf::Sprite sprite_buton ;
public :
	sf::Sprite draw_buton( ) { return sprite_buton ;}
	bool mouse_over( sf::RenderWindow & window );
	bool include_mouse( sf::RenderWindow & window );
	void set( std::string, sf::Vector2f );

	buton( std::string, sf::Vector2f );
	buton( ) { return ;}
};
void buton::set( std::string str, sf::Vector2f vect )
{
	textura_buton.loadFromFile( str );
	sprite_buton.setTexture( textura_buton );
	sprite_buton.setPosition( vect );
}
bool buton::mouse_over( sf::RenderWindow & window )
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition( window );

	if( sprite_buton.getGlobalBounds().contains(( float )mouse_pos.x,( float ) mouse_pos.y ) &&
		!(sf::Mouse::isButtonPressed( sf::Mouse::Left )))
	{
		return true ;
	}
	return false ;
}
bool buton::include_mouse( sf::RenderWindow & window )
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition( window );
	if( mouse_over( window ))
	{
		click = false ;
	}
	if( sprite_buton.getGlobalBounds().contains(( float )mouse_pos.x,( float ) mouse_pos.y ) &&
		sf::Mouse::isButtonPressed( sf::Mouse::Left ) && 
		click == false )
	{
		click = true ;
		return true ;
	}
	return false ;
}

buton::buton( std::string str , sf::Vector2f vect )
{
	textura_buton.loadFromFile( str );
	sprite_buton.setTexture( textura_buton );
	sprite_buton.setPosition( vect );
}

#endif