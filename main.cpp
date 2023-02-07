#include "SFML/Graphics.hpp"
#include "const.hpp"
#include "Tetromino.hpp"
#include <iostream>
#include <array>
#include <chrono>

inline unsigned char &at(map_t &map, int x, int y)
{
	return (map[y * WIDTH + x]);
}

void	colorCell(unsigned char c, sf::RectangleShape &cell)
{
	switch (c)
	{
		case (0):
			cell.setFillColor({38, 35, 110}); break;
		case (1):
			cell.setFillColor({255, 255, 255}); break;
		case (O_SHAPE):
			cell.setFillColor({235, 195, 52}); break;
		case (I_SHAPE):
			cell.setFillColor({52, 225, 235}); break;
		case (T_SHAPE):
			cell.setFillColor({180, 52, 235}); break;
		case (J_SHAPE):
			cell.setFillColor({52, 64, 235}); break;
		case (L_SHAPE):
			cell.setFillColor({235, 131, 52}); break;
		case (S_SHAPE):
			cell.setFillColor({61, 235, 52}); break;
		case (Z_SHAPE):
			cell.setFillColor({235, 52, 52}); break;
		default:
			break;
	}
}

void	drawMatrix(map_t &matrix, sf::RenderWindow &window,
					sf::RectangleShape &cell, Tetromino const &tetromino)
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			colorCell(at(matrix, j, i), cell);
			cell.setPosition(CELL_SIZE * j, CELL_SIZE * i);
			window.draw(cell);
		}
	}
	for (auto &tile : tetromino._tiles)
		if (at(matrix, tile.x, tile.y) != 0)
			return;
	colorCell(tetromino._type, cell);
	for (auto &tile : tetromino._tiles)
	{
		cell.setPosition(CELL_SIZE * tile.x, CELL_SIZE * tile.y);
		window.draw(cell);
	}
}

void	clearLine(map_t &matrix, int y)
{
	for (int j = y; j > 0; --j)
	{
		for (int i = 0; i < WIDTH; ++i)
			at(matrix, i, j) = at(matrix, i, j - 1);
	}
}

void	checkMatrix(map_t &matrix, bool &playing)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		int tiles = 0;
		for (int x = 0; x < WIDTH; ++x)
			tiles += (at(matrix, x, y) != 0);
		if (tiles && !y)
			playing = false;
		if (tiles == WIDTH)
			clearLine(matrix, y);
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH * RESIZE_FACTOR, CELL_SIZE * HEIGHT * RESIZE_FACTOR), "Tetris");
	window.setFramerateLimit(60);
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * WIDTH, CELL_SIZE * HEIGHT)));
	window.setKeyRepeatEnabled(true);

	map_t matrix = {};

	unsigned char timer = 0, gamespeed = 10;
	const unsigned char shapes[] = {I_SHAPE, O_SHAPE, T_SHAPE, L_SHAPE, J_SHAPE, Z_SHAPE, S_SHAPE};

	sf::Texture texture;
	sf::Sprite	sprite;
	sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
	Tetromino tetromino({WIDTH / 2, 5}, shapes[time(NULL) % 6], 0);
	bool playing = true;
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
			switch (event.type)
			{
				case (sf::Event::Closed):
					window.close(); break;
				case (sf::Event::MouseButtonPressed):
				{
					int x = sf::Mouse::getPosition(window).x / CELL_SIZE / RESIZE_FACTOR;
					int y = sf::Mouse::getPosition(window).y / CELL_SIZE / RESIZE_FACTOR;
					at(matrix, x, y) = 1 - at(matrix, x, y);
					break;
				}
				case (sf::Event::KeyPressed):
					if (event.key.code == sf::Keyboard::Right)
						tetromino.moveRight(matrix);
					else if (event.key.code == sf::Keyboard::Left)
						tetromino.moveLeft(matrix);
					else if (event.key.code == sf::Keyboard::Down)
						gamespeed = 0;
					else if (event.key.code == sf::Keyboard::R)
						tetromino.doRotate(matrix);
					else if (event.key.code == sf::Keyboard::P)
						playing = 0;
					break;
				case (sf::Event::KeyReleased):
					if (event.key.code == sf::Keyboard::Down)
						gamespeed = 10;
				
				default:
					break;
			}
        }

		window.clear();
		drawMatrix(matrix, window, cell, tetromino);
		window.display();
		checkMatrix(matrix, playing);
		if (timer > gamespeed && playing)
		{
			tetromino.moveDown(matrix);
			timer = 0;
		}
		else
			timer++;
		// sf::sleep(sf::milliseconds(200));
    }

    return 0;
}


// c++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -I../includes -Ofast