##
## EPITECH PROJECT, 2026
## ARCADE
## File description:
## Makefile
##

NAME        := arcade

CXX         := g++
CXXFLAGS    := -std=gnu++20 -Wall -Wextra -Iinclude -g -fno-gnu-unique
LDLIBS      := -ldl

# Core
SRC_MAIN    := src/main.cpp
OBJ_MAIN    := $(SRC_MAIN:.cpp=.o)

SRC_CORE    := src/core/Core.cpp \
                src/core/menu.cpp \
                src/core/Player.cpp \
                src/core/leaderboard.cpp
OBJ_CORE    := $(SRC_CORE:.cpp=.o)

# Games
SRC_SNAKE   := src/games/arcade_snake.cpp
SO_SNAKE    := lib/arcade_snake.so

SRC_MINESWEEPER := src/games/arcade_minesweeper.cpp
SO_MINESWEEPER    := lib/arcade_minesweeper.so

GAMES       := $(SO_SNAKE) $(SO_MINESWEEPER)

# Graphicals
SRC_NCURSES := src/graphicals/arcade_ncurses.cpp
SO_NCURSES  := lib/arcade_ncurses.so

SRC_SDL2    := src/graphicals/arcade_sdl2.cpp
SO_SDL2     := lib/arcade_sdl2.so

SRC_SFML    := src/graphicals/arcade_sfml.cpp
SO_SFML     := lib/arcade_sfml.so

GRAPHICALS  := $(SO_NCURSES) $(SO_SDL2) $(SO_SFML)

# Tests
TEST_NAME   := unit_tests
SRC_TESTS   := $(wildcard tests/*.cpp)

RM          := rm -f

all: core games graphicals

core: $(NAME)

$(NAME): $(OBJ_MAIN) $(OBJ_CORE)
	$(CXX) -o $(NAME) $(OBJ_MAIN) $(OBJ_CORE) $(LDLIBS)

games: $(GAMES)

graphicals: $(GRAPHICALS)

lib:
	mkdir -p lib

$(SO_SNAKE): $(SRC_SNAKE) | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $<

$(SO_MINESWEEPER): $(SRC_MINESWEEPER) | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $<

$(SO_NCURSES): $(SRC_NCURSES) | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $< -lncurses

$(SO_SDL2): $(SRC_SDL2) | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $< -lSDL2 -lSDL2_ttf

$(SO_SFML): $(SRC_SFML) | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $< -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ_MAIN) $(OBJ_CORE)
	$(RM) *.gcno *.gcda

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TEST_NAME)
	$(RM) $(GAMES) $(GRAPHICALS)
	$(RM) -r lib/

re:
	$(MAKE) fclean
	$(MAKE) all

tests_run:
	@$(RM) *.gcda *.gcno
	$(CXX) -o $(TEST_NAME) $(SRC_MAIN) $(SRC_TESTS) $(CXXFLAGS) -DUNIT_TEST --coverage -lcriterion
	./$(TEST_NAME)
	gcovr --exclude tests/	
	gcovr --exclude tests/ --txt-metric branch

.PHONY: all core games graphicals clean fclean re tests_run