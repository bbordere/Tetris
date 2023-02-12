#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include "SFML/Graphics.hpp"
#include "const.hpp"
#include <random>

struct game;


typedef std::array<unsigned char, WIDTH * HEIGHT> map_t;
unsigned char	selectNextShape(unsigned const char shapes[]);


unsigned char &at(map_t &map, int x, int y);

class Tetromino
{
	private:
	public:
		sf::Vector2f	_pos;
		std::vector<sf::Vector2f> _tiles;
		unsigned char	_type;
		unsigned char	_nextType;
		int				_rotationState;
	
	public:
		Tetromino(void);
		Tetromino(sf::Vector2f pos, unsigned char type, int rotation);
		void moveDown(map_t &matrix);
		void moveRight(map_t &matrix);
		void moveLeft(map_t &matrix);
		void updateMatrix(map_t &matrix);
		unsigned char reset(void);
		bool basicRotate(bool direction, sf::Vector2f center, map_t &matrix);
		void doRotate(map_t &matrix);
		void switchType(unsigned char const newType);
		std::vector<sf::Vector2f> getTiles(unsigned char type);
};


#endif