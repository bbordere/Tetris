#include "SFML/Graphics.hpp"
#include "const.hpp"
#include "Tetromino.hpp"
#include "struct.hpp"
#include <iostream>
#include <array>
#include <chrono>	

unsigned char &at(map_t &map, int x, int y)
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


void	drawNextShape(game_t &game)
{
	std::vector<sf::Vector2f> nextTiles = game.tetromino.getTiles(game.tetromino._nextType);
	game.cell.setPosition((WIDTH + 3) * (CELL_SIZE), CELL_SIZE * 2 - CELL_SIZE / 2);
	game.cell.setSize({5 * (CELL_SIZE), 5 * (CELL_SIZE )});
	colorCell(0, game.cell);

	game.window->draw(game.cell);

	game.cell.setSize({CELL_SIZE - 1, CELL_SIZE - 1});

	colorCell(game.tetromino._nextType, game.cell);
	for (auto &tile : nextTiles)
	{
		if (game.tetromino._nextType == O_SHAPE)
			tile.x += 0.5;
		else if (game.tetromino._nextType == I_SHAPE)
		{
			tile.x -= 0.5;
			tile.y += 0.5;
		}
		game.cell.setPosition(CELL_SIZE * (tile.x + WIDTH), CELL_SIZE * (tile.y + (HEIGHT / 2) - 7));
		game.window->draw(game.cell);
	}
	game.cell.setPosition({(WIDTH + 2) * CELL_SIZE, (HEIGHT / 2) * CELL_SIZE});
}

void	 drawText(game_t &game, std::string const msg, sf::Vector2f const &pos, float const scale)
{
	static sf::Sprite charSprite;
	static sf::Texture font;
	static bool firstTime = true;
	static short size;
	sf::Vector2f charPos(pos);

	if (firstTime)
	{
		font.loadFromFile("font2.png");
		size = font.getSize().x / 95;
		game.fontSize = size;
		charSprite.setTexture(font);
		firstTime = false;
	}

	for (char const c : msg)
	{
		if (c == '\n')
		{
			charPos.x = pos.x;
			charPos.y += font.getSize().y;
			continue;
		}
		charSprite.setPosition(charPos);
		charSprite.setTextureRect(sf::IntRect(size * (c - ' '), 0, size, font.getSize().y));
		charSprite.setScale({scale, scale});
		charPos.x += size * scale;
		game.window->draw(charSprite);
	}
}

void	drawCenteredText(game_t &game, std::string const text, int initX, int initY, float const scale)
{
	short offset = 0;
	std::size_t len = text.length();
	if (text == "0")
		offset = 0;
	else if (len % 2)
		offset = ((len / 2) * (game.fontSize * scale));
	else
		offset = ((len / 2) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2);
	drawText(game, text, {static_cast<float>(initX - offset), static_cast<float>(initY)}, scale);
}

void	drawScoreLevel(game_t &game)
{
	const float scale = 0.5;
	drawCenteredText(game, "Score:",
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT - 4) * CELL_SIZE) / 2, 0.30);
	drawCenteredText(game, std::to_string(game.score),
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT - 2) * CELL_SIZE) / 2, 0.30);

	drawCenteredText(game, "Level:",
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT + 3) * CELL_SIZE) / 2, 0.30);
	drawCenteredText(game, std::to_string(game.level),
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT + 5) * CELL_SIZE) / 2, 0.30);

	drawCenteredText(game, "Lines:",
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT + 10) * CELL_SIZE) / 2, 0.30);
	drawCenteredText(game, std::to_string(game.nbLineCleared),
					((WIDTH * 1.5) * (game.fontSize * scale)) - ((game.fontSize * scale) / 2),
					((HEIGHT + 12) * CELL_SIZE) / 2, 0.30);
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
		drawNextShape(game);
		drawScoreLevel(game);
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

void	resetMatrix(map_t &matrix, unsigned int &nbLineCleared)
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
		matrix[i] = 0;
	nbLineCleared = 0;
}

void	checkMatrix(map_t &matrix, game_t &game)
{
	unsigned char clearedLines = 0;
	static const unsigned int scores[] = {40, 100, 300, 1200};
	for (int y = 0; y < HEIGHT; ++y)
	{
		int tiles = 0;
		for (int x = 0; x < WIDTH; ++x)
			tiles += (at(matrix, x, y) != 0);
		if (tiles && !y)
		{
			game.reseting = true;
			resetMatrix(matrix, game.nbLineCleared);
			game.score = 0;
		}
		else if (tiles == WIDTH)
		{
			clearLine(matrix, game, y);
			++clearedLines;
		}
	}
	if (clearedLines > 4)
		clearedLines = 4;
	if (clearedLines)
		game.score += scores[clearedLines - 1] * (game.level + 1);
	game.level = game.nbLineCleared / 10;
}


unsigned char	selectNextShape(unsigned const char shapes[])
{
	static std::mt19937 gen(time(NULL));
	return (shapes[gen() % 7]);
}

void	initGame(game_t &game, sf::RenderWindow *window)
{
	game.nbLineCleared = 0;
	game.starting = true;
	game.pausing = false;
	game.reseting = false;
	game.tetromino = Tetromino({WIDTH / 2, 0}, selectNextShape(game.shapes), 0);
	game.cell = sf::RectangleShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
	game.directionPressed = 0;
	game.window = window;
	game.tetromino._nextType = game.shapes[(game.tetromino._type + O_SHAPE + I_SHAPE) % 7];
	game.score = 0;
	game.dt = sf::Time::Zero;
	game.elapsedTime = sf::Time::Zero;
	game.level = 0;
	game.gameSpeed = sf::Time{sf::seconds(50.f / (100.f + (game.level * game.level * 5.f)))};
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

void	textMenu(game_t &game, std::string const msg)
{
	game.window->clear();
	std::size_t nbLine = msg.find('\n');
	(void)nbLine;
	drawText(game, msg, {((WIDTH / 2) * CELL_SIZE), ((HEIGHT / 2) * CELL_SIZE) - (float)(game.fontSize * 0.25)}, 0.25);
	game.window->display();
}

void	inputHandling(game_t &game, map_t &matrix)
{
	sf::Event event;
	while (game.window->pollEvent(event))
	{
		switch (event.type)
		{
			case (sf::Event::Closed):
				game.window->close(); break;
			case (sf::Event::MouseButtonPressed):
			{
				int x = sf::Mouse::getPosition(*game.window).x / CELL_SIZE / RESIZE_FACTOR;
				int y = sf::Mouse::getPosition(*game.window).y / CELL_SIZE / RESIZE_FACTOR;
				at(matrix, x, y) = 1 - at(matrix, x, y);
				break;
			}
			case (sf::Event::KeyPressed):
				if (event.key.code == sf::Keyboard::Enter)
				{
					game.starting = false;
					game.reseting = false;
				}
				if (game.pausing && event.key.code != sf::Keyboard::P)
					break;
				if (event.key.code == sf::Keyboard::Right)
					game.tetromino.moveRight(matrix);
				else if (event.key.code == sf::Keyboard::Left)
					game.tetromino.moveLeft(matrix);
				if (event.key.code == sf::Keyboard::Down)
					game.gameSpeed = sf::Time{sf::seconds(5.f / (150.f + (5.f)))};
				if (event.key.code == sf::Keyboard::J)
					++game.nbLineCleared;
				if (event.key.code == sf::Keyboard::R)
					game.tetromino.doRotate(matrix);
				if (event.key.code == sf::Keyboard::P)
					game.pausing = 1 - game.pausing;
				break;
			case (sf::Event::KeyReleased):
				if (event.key.code == sf::Keyboard::Down)
					game.gameSpeed = sf::Time{sf::seconds(50.f / (100.f + (game.level * game.level * 5.f)))};
				else if (event.key.code == sf::Keyboard::Left
						|| event.key.code == sf::Keyboard::Right)
					game.directionPressed = 0;
			default:
				break;
		}
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH * RESIZE_FACTOR * 2, CELL_SIZE * HEIGHT * RESIZE_FACTOR), "Tetris");
	window.setFramerateLimit(60);
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * WIDTH * 2, CELL_SIZE * HEIGHT)));

	map_t matrix = {};
	game_t game;
	initGame(game, &window);
	std::cout << game.starting << '\n';
	while (window.isOpen())
    {
		game.trigger = sf::Time{game.gameSpeed};
		game.dt = game.clock.restart();
		game.elapsedTime += game.dt;
		inputHandling(game, matrix);
		if (game.starting)
		{
			textMenu(game, "Welcome to tetris\nPress Enter to play");
			continue;
		}
		else if (game.reseting)
		{
			textMenu(game, "Game Over !\nPress Enter to replay");
			continue;
		}
    	window.clear();
		checkMatrix(matrix, game);
		moveTetromino(game, matrix);
		drawScoreLevel(game);
		if (!game.pausing)
		{
			drawNextShape(game);
			drawMatrix(matrix, window, game);
		}
		else
			textMenu(game, "Game Paused\nPress p to continue");
		if (game.elapsedTime > game.trigger && !game.pausing)
		{
			game.tetromino.moveDown(matrix);
			game.elapsedTime = sf::Time::Zero;
		}
		window.display();
		// sf::sleep(sf::milliseconds(200));
    }

    return 0;
}
