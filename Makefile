##
## EPITECH PROJECT, 2026
## ARCADE
## File description:
## Makefile
##

SRC         := src/main.cpp

CORE         := src/core/Core.cpp \
					src/core/menu.cpp

GAMES        := src/games/arcade_snake.cpp

GRAPHICALS   := src/graphicals/arcade_ncurses.cpp \
						src/graphicals/arcade_sdl2.cpp

SO_GAMES     := lib/arcade_snake.so
SO_GRAPH     := lib/arcade_ncurses.so  \
						lib/arcade_sdl2.so
OLD_SO       := src/games/arcade_snake.so \
					src/graphicals/arcade_ncurses.so \
					src/graphicals/arcade_sdl2.so

SRC_TESTS    := $(wildcard tests/*.cpp)

OBJ          := $(SRC:.cpp=.o)
OBJ_CORE     := $(CORE:.cpp=.o)

NAME        := arcade
TEST_NAME   := unit_tests

CXXFLAGS    := -std=gnu++20 -Wall -Wextra -Iinclude -g -fno-gnu-unique
CPPFLAGS    := -Iinclude
VPATH       := src/games src/graphicals

CXX         := g++
RM          := rm -f
LDLIBS      := -ldl
PLUGIN_LIBS := -lSDL2 -lncurses

all: $(NAME)

$(NAME): $(OBJ) $(OBJ_CORE) $(SO_GAMES) $(SO_GRAPH)
	$(CXX) -o $(NAME) $(OBJ) $(OBJ_CORE) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib:
	mkdir -p lib

lib/%.so: %.cpp | lib
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $< $(PLUGIN_LIBS)

core: $(OBJ_CORE)

games: $(SO_GAMES)

graphicals: $(SO_GRAPH)

clean:
	$(RM) $(OBJ) $(OBJ_CORE)
	$(RM) *.gcno *.gcda
	$(RM) $(OLD_SO)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TEST_NAME)
	$(RM) $(SO_GAMES) $(SO_GRAPH)

re: fclean all

tests_run:
	@$(RM) *.gcda *.gcno
	$(CXX) -o $(TEST_NAME) $(SRC) $(SRC_TESTS) $(CXXFLAGS) -DUNIT_TEST --coverage -lcriterion
	./$(TEST_NAME)
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch

.PHONY: all clean fclean re tests_run