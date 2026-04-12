/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper.cpp
*/

#include "../../include/games/arcade_minesweeper.hpp"
#include <cstdlib>
#include <ctime>

/**
 * @brief Initializes the Minesweeper game state based on the selected difficulty.
 * @return An empty std::any (for interface compatibility).
 */
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

/**
 * @brief Updates the game state, handles timer and win/lose conditions.
 * @return An empty std::any (for interface compatibility).
 */
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

/**
 * @brief Handles user input commands for the Minesweeper game.
 * @param inputs List of input commands for the current frame.
 * @return Does not return a value.
 */
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

/**
 * @brief Calculates and returns the current score for the Minesweeper game.
 * @return The current score as an integer.
 */
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

/**
 * @brief Checks if the game is over.
 * @return True if the game is over, false otherwise.
 */
bool arcade_minesweeper::isGameOver() {
    return false; 
}

/**
 * @brief Builds and returns the display data (sprites and texts) for the Minesweeper game.
 * @return The current display data for rendering.
 */
Arcade::DisplayData arcade_minesweeper::getDisplayables() const {
    Arcade::DisplayData data;
    int offsetX = 5;
    int offsetY = 5;

    Arcade::TextEntity title;
    title.content = "MINESWEEPER";
    title.x = offsetX + _width / 2 - 4;
    title.y = 2;
    title.color = Arcade::Color::YELLOW;
    data.texts.push_back(title);
    std::string bombCount = "Mines: " + std::to_string(_totalBombs - _flagsPlaced);
    Arcade::TextEntity bombText;
    bombText.content = bombCount;
    bombText.x = offsetX;
    bombText.y = 3;
    bombText.color = Arcade::Color::WHITE;
    data.texts.push_back(bombText);
    std::string diffStr = (_difficulty == 0) ? "EASY" : (_difficulty == 1) ? "MEDIUM" : "HARD";
    Arcade::TextEntity diffText;
    diffText.content = "Diff: " + diffStr;
    diffText.x = offsetX;
    diffText.y = 4;
    diffText.color = Arcade::Color::CYAN;
    data.texts.push_back(diffText);
    std::string timeStr = "Time: " + std::to_string(_remainingTime) + "s";
    Arcade::TextEntity timeText;
    timeText.content = timeStr;
    timeText.x = offsetX + _width - 6;
    timeText.y = 3;
    timeText.color = Arcade::Color::WHITE;
    data.texts.push_back(timeText);
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
    Arcade::TextEntity scoreText;
    scoreText.content = scoreStr;
    scoreText.x = offsetX + _width - 6;
    scoreText.y = 4;
    scoreText.color = Arcade::Color::MAGENTA;
    data.texts.push_back(scoreText);
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
            Arcade::TextEntity winText1;
            winText1.content = "  YOU  ";
            winText1.x = rightX;
            winText1.y = textY;
            winText1.color = Arcade::Color::GREEN;
            data.texts.push_back(winText1);
            Arcade::TextEntity winText2;
            winText2.content = " WIN !!";
            winText2.x = rightX;
            winText2.y = textY + 1;
            winText2.color = Arcade::Color::GREEN;
            data.texts.push_back(winText2);
        } else {
            Arcade::TextEntity loseText1;
            loseText1.content = " GAME ";
            loseText1.x = rightX;
            loseText1.y = textY;
            loseText1.color = Arcade::Color::RED;
            data.texts.push_back(loseText1);
            Arcade::TextEntity loseText2;
            loseText2.content = "OVER !";
            loseText2.x = rightX;
            loseText2.y = textY + 1;
            loseText2.color = Arcade::Color::RED;
            data.texts.push_back(loseText2);
        }
        Arcade::TextEntity restartText;
        restartText.content = "Press R to restart";
        restartText.x = rightX - 2;
        restartText.y = textY + 4;
        restartText.color = Arcade::Color::WHITE;
        data.texts.push_back(restartText);
        Arcade::TextEntity menuText;
        menuText.content = "Press ESC for menu";
        menuText.x = rightX - 2;
        menuText.y = textY + 5;
        menuText.color = Arcade::Color::WHITE;
        data.texts.push_back(menuText);
    }

    return data;
}

/**
 * @brief Generates the bomb field, avoiding the first clicked cell and its neighbors.
 * @param safe_x X coordinate of the safe cell.
 * @param safe_y Y coordinate of the safe cell.
 * @return Does not return a value.
 */
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

/**
 * @brief Reveals a cell and triggers game logic (bomb, empty, or neighbor reveal).
 * @param coord_x X coordinate of the cell.
 * @param coord_y Y coordinate of the cell.
 * @return Does not return a value.
 */
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

/**
 * @brief Recursively reveals all neighboring empty cells.
 * @param coord_x X coordinate of the starting cell.
 * @param coord_y Y coordinate of the starting cell.
 * @return Does not return a value.
 */
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

/**
 * @brief Checks if a cell contains a bomb.
 * @param coord_x X coordinate of the cell.
 * @param coord_y Y coordinate of the cell.
 * @return True if the cell contains a bomb, false otherwise.
 */
bool arcade_minesweeper::is_bomb(int coord_x, int coord_y) {
    return _grid[coord_y][coord_x] == BOMB;
}

/**
 * @brief Handles the logic when a bomb is triggered (game over, reveal all bombs).
 * @return Does not return a value.
 */
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

/**
 * @brief Ends the game and sets the game over state.
 * @return Does not return a value.
 */
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
        {Arcade::ElementType::WALL, "assets/textures/wall.png"},
        {Arcade::ElementType::FLAG, "assets/textures/flag.png"},
        {Arcade::ElementType::BOMB, "assets/textures/bomb.png"},
        {Arcade::ElementType::PLAYER, "assets/textures/Miner.png"},
        {Arcade::ElementType::EMPTY, "assets/textures/TileEmpty.png"},
        {Arcade::ElementType::ONE, "assets/textures/Tile1.png"},
        {Arcade::ElementType::TWO, "assets/textures/Tile2.png"},
        {Arcade::ElementType::THREE, "assets/textures/Tile3.png"},
        {Arcade::ElementType::FOUR, "assets/textures/Tile4.png"},
        {Arcade::ElementType::FIVE, "assets/textures/Tile5.png"},
        {Arcade::ElementType::SIX, "assets/textures/Tile6.png"},
        {Arcade::ElementType::SEVEN, "assets/textures/Tile7.png"},
        {Arcade::ElementType::EIGHT, "assets/textures/Tile8.png"},
    };
}
