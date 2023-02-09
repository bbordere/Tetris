#include "SFML/Graphics.hpp"
#include "const.hpp"
#include "Tetromino.hpp"
#include "struct.hpp"
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

void	drawMatrix(map_t &matrix, sf::RenderWindow &window, game_t &game)
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			colorCell(at(matrix, j, i), game.cell);
			game.cell.setPosition(CELL_SIZE * j, CELL_SIZE * i);
			window.draw(game.cell);
		}
	}
	for (auto &tile : game.tetromino._tiles)
		if (at(matrix, tile.x, tile.y) != 0)
			return;
	colorCell(game.tetromino._type, game.cell);
	for (auto &tile : game.tetromino._tiles)
	{
		game.cell.setPosition(CELL_SIZE * tile.x, CELL_SIZE * tile.y);
		window.draw(game.cell);
	}
}

void	drawClearedLine(map_t &matrix, game_t &game, int y)
{
	unsigned char timer = 0;
	for (int i = 0; i < WIDTH; ++i)
	{
		at(matrix, i, y) = 1;
	}

	while (timer < 20)
	{
		for (int i = 0; i < HEIGHT; ++i)
		{
			for (int j = 0; j < WIDTH; ++j)
			{
				colorCell(at(matrix, j, i), game.cell);
				if (at(matrix, j, i) == 1 && i == y)
				{
					game.cell.setSize({static_cast<float>((CELL_SIZE - 1) - (timer * 0.25)), static_cast<float>((CELL_SIZE - 1) - (timer * 0.25))});
					game.cell.setPosition((CELL_SIZE * j + (timer * 0.125)), (CELL_SIZE * i + (timer * 0.125)));
				}
				else
				{
					game.cell.setSize({CELL_SIZE - 1, CELL_SIZE - 1});
					game.cell.setPosition((CELL_SIZE * j), (CELL_SIZE * i));
				}
				game.window->draw(game.cell);
			}
		}
		game.window->display();
		sf::sleep(sf::milliseconds(15));
		game.window->clear();
		++timer;
		game.cell.setSize({CELL_SIZE - 1, CELL_SIZE - 1});
	}
}

void	clearLine(map_t &matrix, game_t &game, int y)
{
	drawClearedLine(matrix, game, y);
	for (int j = y; j > 0; --j)
	{
		for (int i = 0; i < WIDTH; ++i)
			at(matrix, i, j) = at(matrix, i, j - 1);
	}
	++game.nbLineCleared;
}

void	resetMatrix(map_t &matrix, unsigned int nbLineCleared)
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
		matrix[i] = 0;
	nbLineCleared = 0;
}

void	checkMatrix(map_t &matrix, game_t &game)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		int tiles = 0;
		for (int x = 0; x < WIDTH; ++x)
			tiles += (at(matrix, x, y) != 0);
		if (tiles && !y)
		{
			game.playing = false;
			resetMatrix(matrix, game.nbLineCleared);
		}
		else if (tiles == WIDTH)
			clearLine(matrix, game, y);
	}
}

void	initGame(game_t &game, sf::RenderWindow *window)
{
	game.timer = 0;
	game.gameSpeed = DEFAULT_GAME_SPEED;
	game.nbLineCleared = 0;
	game.playing = true;
	game.tetromino = Tetromino({WIDTH / 2, 0}, game.shapes[time(NULL) % 6], 0);
	game.cell = sf::RectangleShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
	game.directionPressed = 0;
	game.window = window;
}

void	moveTetromino(game_t &game, map_t &matrix)
{
	switch (game.directionPressed)
	{
		case (RIGHT_DIR):
			game.tetromino.moveRight(matrix); break;
		case (LEFT_DIR):
			game.tetromino.moveLeft(matrix); break;
		default:
			break;
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH * RESIZE_FACTOR, CELL_SIZE * HEIGHT * RESIZE_FACTOR), "Tetris");
	window.setFramerateLimit(60);
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * WIDTH, CELL_SIZE * HEIGHT)));
	window.setKeyRepeatEnabled(true);

	map_t matrix = {};
	game_t game;
	initGame(game, &window);

	sf::Texture texture;
	sf::Sprite	sprite;
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
					if (!game.playing && event.key.code != sf::Keyboard::P)
						break;
					if (event.key.code == sf::Keyboard::Right)
						game.tetromino.moveRight(matrix);
					else if (event.key.code == sf::Keyboard::Left)
						game.tetromino.moveLeft(matrix);
					if (event.key.code == sf::Keyboard::Down)
						game.gameSpeed = 0;
					if (event.key.code == sf::Keyboard::R)
						game.tetromino.doRotate(matrix);
					if (event.key.code == sf::Keyboard::P)
						game.playing = 1 - game.playing;
					break;
				case (sf::Event::KeyReleased):
					if (event.key.code == sf::Keyboard::Down)
						game.gameSpeed = DEFAULT_GAME_SPEED;
					else if (event.key.code == sf::Keyboard::Left
							|| event.key.code == sf::Keyboard::Right)
						game.directionPressed = 0;
				default:
					break;
			}
        }

		window.clear();
		checkMatrix(matrix, game);
		moveTetromino(game, matrix);			
		if (game.playing)
			drawMatrix(matrix, window, game);
		if (game.timer > game.gameSpeed && game.playing)
		{
			game.tetromino.moveDown(matrix);
			game.timer = 0;
		}
		else
			game.timer++;
		window.display();
		// sf::sleep(sf::milliseconds(200));
    }

    return 0;
}


// c++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -I../includes -Ofast