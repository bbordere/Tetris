# CXX = x86_64-w64-mingw32-g++
CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -I includes/ -g3 -Ilib/include

FILES = main.cpp Tetromino.cpp

OBJS = $(FILES:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

NAME = tetris

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -Ilib/include -Llib/SFML_LINUX/lib -lsfml-graphics -lsfml-window -lsfml-system -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re