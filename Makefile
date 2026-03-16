##
## EPITECH PROJECT, 2026
## ARCADE
## File description:
## Makefile
##

SRC         := src/main.cpp

CORE         := src/core/Core.cpp

GAMES        := $(wildcard src/games/*.cpp)
GRAPHICALS   := $(wildcard src/graphicals/*.cpp)

SO_GAMES     := $(wildcard src/games/*.so) $(GAMES:.cpp=.so)
SO_GRAPH     := $(wildcard src/graphicals/*.so) $(GRAPHICALS:.cpp=.so)

SRC_TESTS    := $(wildcard tests/*.cpp)

OBJ          := $(SRC:.cpp=.o)
OBJ_CORE     := $(CORE:.cpp=.o)

NAME        := arcade
TEST_NAME   := unit_tests

CXXFLAGS    := -Wall -Wextra -Iinclude -g -fno-gnu-unique
CPPFLAGS    := -Iinclude

CXX         := g++
RM          := rm -f

all: $(NAME)

$(NAME): $(OBJ) $(OBJ_CORE) $(SO_GAMES) $(SO_GRAPH)
	$(CXX) -o $(NAME) $(OBJ) $(OBJ_CORE) $(SO_GAMES) $(SO_GRAPH)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.so: %.cpp
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $<

core: $(OBJ_CORE)

games: $(SO_GAMES)

graphicals: $(SO_GRAPH)

clean:
	$(RM) $(OBJ) $(OBJ_CORE)
	$(RM) *.gcno *.gcda

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
	gcovr --exclude tests/ --branches

.PHONY: all clean fclean re tests_run