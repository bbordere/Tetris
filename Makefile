# CXX = x86_64-w64-mingw32-g++
CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -I includes/ -Ilib/SFML_LINUX/include -Llib/SFML_LINUX/lib -lsfml-graphics -lsfml-window -lsfml-system -g3

FILES = main.cpp Tetromino.cpp

OBJS = $(FILES:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

NAME = tetris

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re