/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper.cpp
*/

#include "../../include/games/arcade_minesweeper.hpp"
#include <cstdlib>
#include <ctime>

std::any arcade_minesweeper::init() {
    if (_difficulty == 0) {
        _width = 16;
        _height = 16;
        _totalBombs = 10;
        _timeLimit = 300;
    } else if (_difficulty == 1) {
        _width = 18;
        _height = 18;
        _totalBombs = 40;
        _timeLimit = 600;
    } else {
        _width = 24;
        _height = 24;
        _totalBombs = 100;
        _timeLimit = 900;
    }
    _flagsPlaced = 0;
    _isGameOver = false;
    _hasWon = false;
    _firstClick = true;
    _cursorX = _width / 2;
    _cursorY = _height / 2;
    _remainingTime = _timeLimit;
    std::srand(std::time(nullptr));    
    _grid.assign(_height, std::vector<int>(_width, 0));
    _revealed.assign(_height, std::vector<bool>(_width, false));
    _flagged.assign(_height, std::vector<bool>(_width, false));
    _startTime = std::chrono::steady_clock::now();
    return {};
}

std::any arcade_minesweeper::update() {
    if (_isGameOver)
        return {};

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();

    _remainingTime = _timeLimit - static_cast<int>(elapsed);
    if (_remainingTime <= 0) {
        _remainingTime = 0;
        _isGameOver = true;
        _hasWon = false;
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                if (_grid[y][x] == BOMB) 
                    _revealed[y][x] = true;
            }
        }
        return {};
    }
    int hiddenNonBombs = 0;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_grid[y][x] != BOMB && !_revealed[y][x])
                hiddenNonBombs++;
        }
    }
    if (hiddenNonBombs == 0 && !_firstClick) {
        _hasWon = true;
        end_game();
    }
    return {};
}

void arcade_minesweeper::handleKey(const std::vector<Arcade::InputCommand>& inputs) {
    for (auto input : inputs) {
        if (input == Arcade::InputCommand::SWITCH_DIFFICULTY) {
            _difficulty = (_difficulty + 1) % 3;
            init();
            return;
        }
    }
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
            if (!_flagged[_cursorY][_cursorX])
                reveal_case(_cursorX, _cursorY);
        } else if (input == Arcade::InputCommand::FLAG) {
            if (!_revealed[_cursorY][_cursorX]) {
                _flagged[_cursorY][_cursorX] = !_flagged[_cursorY][_cursorX];
                _flagsPlaced += _flagged[_cursorY][_cursorX] ? 1 : -1;
            }
        }
    }
}

void arcade_minesweeper::handleMouse(Arcade::MouseEvent inputs)
{
    (void)inputs;
}

int arcade_minesweeper::getScore()
{
    int revealed = 0;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_revealed[y][x] && _grid[y][x] != BOMB)
                ++revealed;
        }
    } 
    int score = revealed * 10;

    if (_hasWon)
        score += _remainingTime;    
    return score;
}

bool arcade_minesweeper::isGameOver() {
    return false; 
}

Arcade::DisplayData arcade_minesweeper::getDisplayables() const {
    Arcade::DisplayData data;
    int offsetX = 5;
    int offsetY = 5;

    data.texts.push_back({"MINESWEEPER", offsetX + _width / 2 - 4, 2, Arcade::Color::YELLOW});
    std::string bombCount = "Mines: " + std::to_string(_totalBombs - _flagsPlaced);
    data.texts.push_back({bombCount, offsetX, 3, Arcade::Color::WHITE});
    std::string diffStr = (_difficulty == 0) ? "EASY" : (_difficulty == 1) ? "MEDIUM" : "HARD";
    data.texts.push_back({"Diff: " + diffStr, offsetX, 4, Arcade::Color::CYAN});
    std::string timeStr = "Time: " + std::to_string(_remainingTime) + "s";
    data.texts.push_back({timeStr, offsetX + _width - 6, 3, Arcade::Color::WHITE});
    int currentScore = 0;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_revealed[y][x] && _grid[y][x] != BOMB) {
                currentScore += 10;
            }
        }
    }
    if (_hasWon)
        currentScore += _remainingTime;
    std::string scoreStr = "Score: " + std::to_string(currentScore);
    data.texts.push_back({scoreStr, offsetX + _width - 6, 4, Arcade::Color::MAGENTA});
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            Arcade::SpriteObject sprite;
            sprite.x = x + offsetX;
            sprite.y = y + offsetY;
            if (_revealed[y][x]) {
                if (_grid[y][x] == BOMB) {
                    sprite.type = Arcade::ElementType::BOMB;
                } else if (_grid[y][x] == 0) {
                    sprite.type = Arcade::ElementType::EMPTY;
                } else {
                    switch (_grid[y][x]) {
                        case 1: sprite.type = Arcade::ElementType::ONE; break;
                        case 2: sprite.type = Arcade::ElementType::TWO; break;
                        case 3: sprite.type = Arcade::ElementType::THREE; break;
                        case 4: sprite.type = Arcade::ElementType::FOUR; break;
                        case 5: sprite.type = Arcade::ElementType::FIVE; break;
                        case 6: sprite.type = Arcade::ElementType::SIX; break;
                        case 7: sprite.type = Arcade::ElementType::SEVEN; break;
                        case 8: sprite.type = Arcade::ElementType::EIGHT; break;
                        default: sprite.type = Arcade::ElementType::EMPTY; break;
                    }
                }
            } else if (_flagged[y][x])
                sprite.type = Arcade::ElementType::FLAG;
            else
                sprite.type = Arcade::ElementType::WALL;
            data.sprites.push_back(sprite);
            if (!_isGameOver && x == _cursorX && y == _cursorY) {
                Arcade::SpriteObject cursor;
                cursor.x = x + offsetX;
                cursor.y = y + offsetY;
                cursor.type = Arcade::ElementType::PLAYER;
                data.sprites.push_back(cursor);
            }
        }
    }
    if (_isGameOver) {
        int rightX = offsetX + _width + 4;
        int textY = offsetY + (_height / 2) - 2;

        if (_hasWon) {
            data.texts.push_back({"  YOU  ", rightX, textY, Arcade::Color::GREEN});
            data.texts.push_back({" WIN !!", rightX, textY + 1, Arcade::Color::GREEN});
        } else {
            data.texts.push_back({" GAME ", rightX, textY, Arcade::Color::RED});
            data.texts.push_back({"OVER !", rightX, textY + 1, Arcade::Color::RED});
        }
        data.texts.push_back({"Press R to restart", rightX - 2, textY + 4, Arcade::Color::WHITE});
        data.texts.push_back({"Press ESC for menu", rightX - 2, textY + 5, Arcade::Color::WHITE});
    }

    return data;
}

void arcade_minesweeper::generate_bomb_field(int safe_x, int safe_y) {
    int bombsPlaced = 0;

    while (bombsPlaced < _totalBombs) {
        int x = std::rand() % _width;
        int y = std::rand() % _height;

        if (std::abs(x - safe_x) <= 1 && std::abs(y - safe_y) <= 1)
            continue;
        if (_grid[y][x] != BOMB) {
            _grid[y][x] = BOMB;
            bombsPlaced++;
        }
    }

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_grid[y][x] != BOMB) {
                int count = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                            if (_grid[ny][nx] == BOMB)
                                count++;
                        }
                    }
                }
                _grid[y][x] = count;
            }
        }
    }
}

void arcade_minesweeper::reveal_case(int coord_x, int coord_y) {
    if (coord_x < 0 || coord_x >= _width || coord_y < 0 || coord_y >= _height)
        return;
    if (_revealed[coord_y][coord_x] || _flagged[coord_y][coord_x])
        return;
    if (_firstClick) {
        generate_bomb_field(coord_x, coord_y);
        _firstClick = false;
    }

    _revealed[coord_y][coord_x] = true;
    if (is_bomb(coord_x, coord_y)) {
        splode_bomb();
    } else if (_grid[coord_y][coord_x] == 0) {
        reveal_neighbor_case(coord_x, coord_y);
    }
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
    return _grid[coord_y][coord_x] == BOMB;
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

extern "C" {
    int getType() {
        return 2;
    }

    Arcade::IGames* entryPoint() {
        return new arcade_minesweeper();
    }
}

std::unordered_map<Arcade::ElementType, std::string> arcade_minesweeper::getAssets() {
    return {
        {Arcade::ElementType::WALL, "asset/textures/wall.png"},
        {Arcade::ElementType::FLAG, "asset/textures/flag.png"},
        {Arcade::ElementType::BOMB, "asset/textures/bomb.png"},
        {Arcade::ElementType::PLAYER, "asset/textures/cursor.png"},
        {Arcade::ElementType::EMPTY, "asset/textures/empty.png"},
        {Arcade::ElementType::ONE, "asset/textures/Tile1.png"},
        {Arcade::ElementType::TWO, "asset/textures/Tile2.png"},
        {Arcade::ElementType::THREE, "asset/textures/Tile3.png"},
        {Arcade::ElementType::FOUR, "asset/textures/Tile4.png"},
        {Arcade::ElementType::FIVE, "asset/textures/Tile5.png"},
        {Arcade::ElementType::SIX, "asset/textures/Tile6.png"},
        {Arcade::ElementType::SEVEN, "asset/textures/Tile7.png"},
        {Arcade::ElementType::EIGHT, "asset/textures/Tile8.png"},
    };
}
