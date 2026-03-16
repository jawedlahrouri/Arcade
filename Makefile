##
## EPITECH PROJECT, 2026
## ARCADE
## File description:
## Makefile
##


SRC         =   src/main.cpp \


SRC_TESTS   =   $(wildcard tests/*.cpp)

OBJ         =   $(SRC:.cpp=.o) $(SRC_MAIN:.cpp=.o)

NAME        =   arcade
TEST_NAME   =   unit_tests

CXXFLAGS    =   -Wall -Wextra -Iinclude -g
CPPFLAGS    =   -Iinclude

CXX         =   g++
RM          =   rm -f

all:    $(NAME)

$(NAME):    $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)
	$(RM) *.gcno *.gcda

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TEST_NAME)

re: fclean all

tests_run:
	@$(RM) *.gcda *.gcno 
	$(CXX) -o $(TEST_NAME) $(SRC) $(SRC_TESTS) $(SRC_MAIN) $(CXXFLAGS) -DUNIT_TEST --coverage -lcriterion
	./$(TEST_NAME)
	gcovr --exclude tests/
	gcovr --exclude tests/ --branches

.PHONY: all clean fclean re tests_run