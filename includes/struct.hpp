#ifndef __STRUCT_HPP__
#define __STRUCT_HPP__

#include "const.hpp"
#include "Tetromino.hpp"
#include "SFML/Graphics.hpp"

typedef struct game
{
	unsigned int nbLineCleared;
	inline static const unsigned char shapes[] = {I_SHAPE, O_SHAPE, T_SHAPE,
												L_SHAPE, J_SHAPE, Z_SHAPE, S_SHAPE};
	bool starting;
	bool pausing;
	bool reseting;
	Tetromino tetromino;
	sf::RectangleShape cell;
	sf::RenderWindow *window;
	unsigned char	directionPressed;
	unsigned int	score;
	unsigned int	level;
	short			fontSize;
	sf::Clock		clock;
	sf::Time		dt;
	sf::Time		elapsedTime;
	sf::Time		trigger;
	sf::Time		gameSpeed;

}	game_t;


#endif