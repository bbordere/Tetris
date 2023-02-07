#include "Tetromino.hpp"
#include "const.hpp"
#include <iostream>

Tetromino::Tetromino(void):
			_pos(sf::Vector2f(WIDTH / 2, 0)), _tiles({
												{WIDTH / 2 - 1, 0}, {WIDTH / 2, 0}, 
												{WIDTH / 2, 1}, {WIDTH / 2 - 1, 1}
											}),_type(O_SHAPE), _rotationState(0) {}

Tetromino::Tetromino(sf::Vector2f pos, unsigned char type, int rotation):
			_pos(pos), _type(type), _rotationState(rotation)
{
	switch (_type)
	{
		case (O_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x, _pos.y + 1}, {_pos.x - 1, _pos.y + 1}}; break;
		case (I_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x + 1, _pos.y}, {_pos.x + 2, _pos.y}}; break;
		case (T_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x + 1, _pos.y}, {_pos.x, _pos.y + 1}}; break;
		case (L_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x + 1, _pos.y}, {_pos.x - 1, _pos.y + 1}}; break;
		case (J_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x + 1, _pos.y}, {_pos.x + 1, _pos.y + 1}}; break;
		case (Z_SHAPE):
			_tiles = {{_pos.x - 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x, _pos.y + 1}, {_pos.x + 1, _pos.y + 1}}; break;
		case (S_SHAPE):
			_tiles = {{_pos.x + 1, _pos.y}, {_pos.x, _pos.y}, {_pos.x, _pos.y + 1}, {_pos.x - 1, _pos.y + 1}}; break;
		default:
			break;
	}
}

void Tetromino::moveDown(map_t &matrix)
{
	for (auto &tile : _tiles)
	{
		if (tile.y + 1 >= HEIGHT || at(matrix, tile.x, tile.y + 1))
		{
			updateMatrix(matrix);
			reset();
			return;
		}
	}
	for (auto &tile : _tiles)
		tile.y += 1;
}

void Tetromino::moveRight(map_t &matrix)
{
	for (auto &tile : _tiles)
	{
		if (tile.x + 1 >= WIDTH
			|| at(matrix, tile.x + 1, tile.y))
			return;
	}
	for (auto &tile : _tiles)
		tile.x += 1;
}

void Tetromino::moveLeft(map_t &matrix)
{
	for (auto &tile : _tiles)
	{
		if (tile.x - 1 < 0
			|| at(matrix, tile.x - 1, tile.y))
			return;
	}
	for (auto &tile : _tiles)
		tile.x -= 1;
}

void Tetromino::updateMatrix(map_t &matrix)
{
	for (auto &tile : _tiles)
		at(matrix, tile.x, tile.y) = _type;
}

void Tetromino::reset(void)
{
	static const unsigned char shapes[] = {I_SHAPE, O_SHAPE, T_SHAPE, L_SHAPE, J_SHAPE, Z_SHAPE, S_SHAPE};
	static std::random_device rd;
	std::uniform_int_distribution<int> index(0, 6);
	_type = shapes[index(rd)];
	switch (_type)
	{
		case (O_SHAPE):
			_tiles = {{WIDTH / 2 - 1, 0}, {WIDTH / 2, 0}, {WIDTH / 2, 1}, {WIDTH / 2 - 1, 1}}; break;
		case (I_SHAPE):
			_tiles = {{WIDTH / 2 - 1, 0}, {WIDTH / 2, 0}, {WIDTH / 2 + 1, 0}, {WIDTH / 2 + 2, 0}}; break;
		case (T_SHAPE):
			_tiles = {{WIDTH / 2  - 1, 0}, {WIDTH / 2 , 0}, {WIDTH / 2  + 1, 0}, {WIDTH / 2, 1}}; break;
		case (L_SHAPE):
			_tiles = {{WIDTH / 2  - 1, 0}, {WIDTH / 2 , 0}, {WIDTH / 2  + 1, 0}, {WIDTH / 2 - 1, 1}}; break;
		case (J_SHAPE):
			_tiles = {{WIDTH / 2  - 1, 0}, {WIDTH / 2 , 0}, {WIDTH / 2  + 1, 0}, {WIDTH / 2 + 1, 1}}; break;
		case (Z_SHAPE):
			_tiles = {{WIDTH / 2  - 1, 0}, {WIDTH / 2 , 0}, {WIDTH / 2, + 1}, {WIDTH / 2 + 1, 1}}; break;
		case (S_SHAPE):
			_tiles = {{WIDTH / 2  + 1, 0}, {WIDTH / 2 , 0}, {WIDTH / 2, + 1}, {WIDTH / 2 - 1, 1}}; break;
	}
	_rotationState = 0;
}

bool Tetromino::basicRotate(bool direction, sf::Vector2f center, map_t &matrix)
{
	if (_type == O_SHAPE)
		return (false);
	if (direction)
	{
		//cos (90) = 0;
		// sin(90) = 1;
		float newX;
		float newY;
		for (auto tile : _tiles)
		{
			tile -= center;
			newX = -tile.y;
			newY = tile.x;
			if (newX + center.x >= WIDTH || newX + center.x < 0 
				|| newY + center.y >= HEIGHT|| newY + center.y < 0
				|| at(matrix, newX + center.x, newY + center.y))
					return (false);
		}
		for (auto &tile : _tiles)
		{
			tile -= center;
			newX = -tile.y;
			newY = tile.x;
			tile = {newX + center.x, newY + center.y};
		}
		_rotationState = ((_rotationState + 1) % 4);
		return (true);
	}
	return (false);
}

void Tetromino::doRotate(map_t &matrix)
{
	if (basicRotate(1, _tiles[1], matrix))
		return;
	static std::array<sf::Vector2f, 4> kickTests;
	switch (_rotationState)
	{
		case (1):
			kickTests = {sf::Vector2f(-1, 0), sf::Vector2f(-1, -1),
						sf::Vector2f(0, 2), sf::Vector2f(-1, 2)};
			break;
		case (2):
			kickTests = {sf::Vector2f(1, 0), sf::Vector2f(1, 1),
						sf::Vector2f(0, -2), sf::Vector2f(1, -2)};
			break;
		case (3):
			kickTests = {sf::Vector2f(1, 0), sf::Vector2f(1, -1),
						sf::Vector2f(0, 2), sf::Vector2f(1, 2)};
			break;
		case (4):
			kickTests = {sf::Vector2f(-1, 0), sf::Vector2f(-1, 1),
						sf::Vector2f(0, -2), sf::Vector2f(-1, -2)};
			break;
		default:
			break;
	}
	for (auto &vector : kickTests)
	{
		if (basicRotate(1, _tiles[1] + vector, matrix))
			return;
	}

}

