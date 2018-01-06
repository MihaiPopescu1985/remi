#ifndef CARTI_H
#define CARTI_H

#include <string>
#include "Enum.cpp"
#include <SFML\Graphics.hpp>

class Carti
{
public:
	void set_carte();
	void drag_carte( sf::RenderWindow & window );
	Carti( );
	~Carti();

public:
	std::string numar_carte ;
	std::string culoare_carte ;
	std::string imagine_carte ;
	Etapa etapa_carte ;
	sf::Texture textura_carte ;
	sf::Sprite sprite_carte ;
	int n ;
	bool pozitie_corecta ;
	sf::Vector2f pozitie_curenta ;
};
void Carti::drag_carte( sf::RenderWindow & window )
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition( window );
	if( sprite_carte.getGlobalBounds().contains(( float )mouse_pos.x,( float ) mouse_pos.y ) && etapa_carte == pe_tabla )
	{
		if( !sf::Mouse::isButtonPressed( sf::Mouse::Left ))
		{
			etapa_carte = is_dragging ;
			pozitie_curenta = sprite_carte.getPosition() ;
		}
	}
	if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && etapa_carte == is_dragging )
	{
		sprite_carte.setPosition(( float )mouse_pos.x - 34,( float ) mouse_pos.y - 34 );
	}
	if( !sprite_carte.getGlobalBounds().contains(( float )mouse_pos.x,( float ) mouse_pos.y ) && etapa_carte == is_dragging )
	{
		etapa_carte = pe_tabla ;
		pozitie_corecta = false ;
	}
	return ;
}

Carti::Carti( )
	: imagine_carte( "poze/blank.png" )
{
	etapa_carte = pe_masa ;
	textura_carte.loadFromFile( imagine_carte );
	sprite_carte.setTexture( textura_carte );
}

Carti::~Carti()
{
}

void Carti::set_carte()
{
	int nr, cl, m ;

	m = n ;

	if( m > 52 )
	{
		m -= 53 ;
	}
		
	nr = m % 13 ;
	cl = m / 13 ;

	if( m == 52 )
	{
		nr = 13 ;
		cl = 3 ;
	}

	switch (nr)
	{
	case (0) :
		numar_carte = "unu" ;
		break ;
	case (1) :
		numar_carte = "doi" ;
		break ;
	case (2) :
		numar_carte = "trei" ;
		break ;
	case (3) :
		numar_carte = "patru" ;
		break ;
	case (4) :
		numar_carte = "cinci" ;
		break ;
	case (5) :
		numar_carte = "sase" ;
		break ;
	case (6) :
		numar_carte = "sapte" ;
		break ;
	case (7) :
		numar_carte = "opt" ;
		break ;
	case (8) :
		numar_carte = "noua" ;
		break ;
	case (9) :
		numar_carte = "zece" ;
		break ;
	case (10) :
		numar_carte = "unsprezece" ;
		break ;
	case (11) :
		numar_carte = "doisprezece" ;
		break ;
	case (12) :
		numar_carte = "treisprezece" ;
		break ;
	default:
		numar_carte = "jolly" ;
		break ;
	}
	switch (cl)
	{
	case (0) :
		culoare_carte = "rosu" ;
		break ;
	case (1) :
		culoare_carte = "galben" ;
		break ;
	case (2) :
		culoare_carte = "albastru" ;
		break ;
	default:
		culoare_carte = "negru" ;
		break;
	}
	imagine_carte = "poze/" + numar_carte + "_" + culoare_carte + ".png" ;
	textura_carte.loadFromFile( imagine_carte );
	sprite_carte.setTexture( textura_carte );
	return ;
}
#endif // CARTI_H