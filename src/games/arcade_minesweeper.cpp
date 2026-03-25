/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper.cpp
*/

#include "../../include/games/arcade_minesweeper.hpp"
#include <cstdlib>
#include <ctime>

void arcade_minesweeper::init() {
    _width = 10;
    _height = 10;
    _difficulty = 0;
    _totalBombs = 10;
    _isGameOver = false;
    _hasWon = false;
    _cursorX = 0;
    _cursorY = 0;

    std::srand(std::time(nullptr));
    _grid = generate_bomb_field(_difficulty);
    _revealed.assign(_height, std::vector<bool>(_width, false));
    _flagged.assign(_height, std::vector<bool>(_width, false));
}

void arcade_minesweeper::update() {
    int hiddenNonBombs = 0;
    
    if (_isGameOver)
        return;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_grid[y][x] != BOMB && !_revealed[y][x])
                hiddenNonBombs++;
        }
    }
    if (hiddenNonBombs == 0) {
        _hasWon = true;
        end_game();
    }
}

void arcade_minesweeper::handleInput(const std::vector<Arcade::InputCommand>& inputs) {
    if (_isGameOver)
        return;
    for (auto input : inputs) {
        if (input == Arcade::InputCommand::UP && _cursorY > 0)
            _cursorY--;
        else if (input == Arcade::InputCommand::DOWN && _cursorY < _height - 1)
            _cursorY++;
        else if (input == Arcade::InputCommand::LEFT && _cursorX > 0)
            _cursorX--;
        else if (input == Arcade::InputCommand::RIGHT && _cursorX < _width - 1)
            _cursorX++;
        else if (input == Arcade::InputCommand::ENTER) {
            if (!_flagged[_cursorY][_cursorX] && !_revealed[_cursorY][_cursorX])
                reveal_case(_cursorX, _cursorY);
        } else if (input == Arcade::InputCommand::FLAG) {
            if (!_revealed[_cursorY][_cursorX])
                _flagged[_cursorY][_cursorX] = !_flagged[_cursorY][_cursorX];
        } else if (input == Arcade::InputCommand::BACK || input == Arcade::InputCommand::QUIT) {
            _isGameOver = true;
            return;
        }
    }
}

bool arcade_minesweeper::isGameOver() {
    return _isGameOver;
}

std::vector<Arcade::SpriteObject> arcade_minesweeper::getDisplayables() const {
    std::vector<Arcade::SpriteObject> displayables;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            Arcade::SpriteObject sprite;
            sprite.x = x;
            sprite.y = y;
            if (x == _cursorX && y == _cursorY)
                sprite.type = Arcade::ElementType::PLAYER;
            else if (_revealed[y][x]) {
                if (_grid[y][x] == BOMB)
                    sprite.type = Arcade::ElementType::ENEMY;
                else if (_grid[y][x] > 0)
                    sprite.type = Arcade::ElementType::SCORE;
                else
                    sprite.type = Arcade::ElementType::EMPTY;
            } else if (_flagged[y][x])
                sprite.type = Arcade::ElementType::FOOD;
            else
                sprite.type = Arcade::ElementType::WALL;
            displayables.push_back(sprite);
        }
    }
    return displayables;
}

void arcade_minesweeper::create_starting_zone() {
}

std::vector<std::vector<int>> arcade_minesweeper::generate_bomb_field(int difficulty) {
    std::vector<std::vector<int>> newGrid(_height, std::vector<int>(_width, 0));
    _totalBombs = (difficulty == 0) ? 10 : (difficulty == 1) ? 20 : 40;
    int bombsPlaced = 0;

    while (bombsPlaced < _totalBombs) {
        int x = std::rand() % _width;
        int y = std::rand() % _height;

        if (newGrid[y][x] != BOMB) {
            newGrid[y][x] = BOMB;
            bombsPlaced++;
        }
    }

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (newGrid[y][x] != BOMB) {
                int count = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < _width && ny >= 0 && ny < _height)
                            if (newGrid[ny][nx] == BOMB) count++;
                    }
                }
                newGrid[y][x] = count;
            }
        }
    }
    return newGrid;
}

int arcade_minesweeper::change_difficulty(int counter) {
    _difficulty = counter % 3;
    return _difficulty;
}

int arcade_minesweeper::get_px(int coord_x) {
    return coord_x;
}

int arcade_minesweeper::get_py(int coord_y) {
    return coord_y;
}

int arcade_minesweeper::count_bomb(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return 0;
    if (_grid[coord_y][coord_x] == BOMB)
        return BOMB;
    return _grid[coord_y][coord_x];
}

void arcade_minesweeper::reveal_case(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return;
    if (_revealed[coord_y][coord_x] || _flagged[coord_y][coord_x])
        return;
    _revealed[coord_y][coord_x] = true;
    if (is_bomb(coord_x, coord_y))
        splode_bomb();
    else if (_grid[coord_y][coord_x] == 0)
        reveal_neighbor_case(coord_x, coord_y);
}

int arcade_minesweeper::remove_empty(int coord_x, int coord_y, int **field) {
    (void)coord_x;
    (void)coord_y;
    (void)field;
    return 0;
}

void arcade_minesweeper::reveal_neighbor_case(int coord_x, int coord_y) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = coord_x + dx;
            int ny = coord_y + dy;
            if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                 if (nx != coord_x || ny != coord_y)
                     reveal_case(nx, ny);
            }
        }
    }
}

bool arcade_minesweeper::is_bomb(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return false;
    return _grid[coord_y][coord_x] == BOMB;
}

bool arcade_minesweeper::is_reveal(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return false;
    return _revealed[coord_y][coord_x];
}

bool arcade_minesweeper::is_flag(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return false;
    return _flagged[coord_y][coord_x];
}

void arcade_minesweeper::splode_bomb() {
    _isGameOver = true;
    _hasWon = false;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_grid[y][x] == BOMB)
                _revealed[y][x] = true;
        }
    }
}

void arcade_minesweeper::end_game() {
    _isGameOver = true;
}

int arcade_minesweeper::count_field() {
    int total = 0;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_grid[y][x] == BOMB)
                total++;
        }
    }
    return total;
}

extern "C" IGames* createGame() {
    return new arcade_minesweeper();
}
