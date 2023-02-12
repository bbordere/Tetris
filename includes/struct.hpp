#ifndef __STRUCT_HPP__
#define __STRUCT_HPP__

#include "const.hpp"
#include "Tetromino.hpp"
#include "SFML/Graphics.hpp"

typedef struct game
{
	unsigned char timer;
	unsigned char gameSpeed;
	unsigned int nbLineCleared;
	inline static const unsigned char shapes[] = {I_SHAPE, O_SHAPE, T_SHAPE,
												L_SHAPE, J_SHAPE, Z_SHAPE, S_SHAPE};
	bool playing;
	Tetromino tetromino;
	sf::RectangleShape cell;
	sf::RenderWindow *window;
	unsigned char	directionPressed;

}	game_t;


#endif