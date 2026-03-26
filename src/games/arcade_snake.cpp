/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/
#include "games/arcade_snake.hpp"
#include <algorithm>
#include <random>

SnakeGame::SnakeGame()
{
    _score = 0;
    _mapWidth = 17;
    _mapHeigth = 15;
    _body = {{8, 4}, {7, 4}, {6, 4}, {5, 4}};
    _apple = {13, 8};
    _currentDirection = Arcade::InputCommand::RIGHT;
    _isGameOver = false;
}

SnakeGame::~SnakeGame()
{
}

int SnakeGame::getScore()
{
    return _score;
}

void SnakeGame::handleKey(const std::vector<Arcade::InputCommand>& inputs)
{
    for (auto command : inputs) {
        if (command == Arcade::InputCommand::UP && _currentDirection != Arcade::InputCommand::DOWN)
            _currentDirection = Arcade::InputCommand::UP;
        else if (command == Arcade::InputCommand::DOWN && _currentDirection != Arcade::InputCommand::UP)
            _currentDirection = Arcade::InputCommand::DOWN;
        else if (command == Arcade::InputCommand::LEFT && _currentDirection != Arcade::InputCommand::RIGHT)
            _currentDirection = Arcade::InputCommand::LEFT;
        else if (command == Arcade::InputCommand::RIGHT && _currentDirection != Arcade::InputCommand::LEFT)
            _currentDirection = Arcade::InputCommand::RIGHT;
    }
}

void SnakeGame::spawnApple()
{
    bool onBody = true;

    while (onBody) {
        _apple.first = rand() % _mapWidth;
        _apple.second = rand() % _mapHeigth;
        onBody = false;
        for (const auto& part : _body) {
            if (part.first == _apple.first && part.second == _apple.second) {
                onBody = true;
                break;
            }
        }
    }
}

std::any SnakeGame::update()
{
    std::pair<int, int> snakeHead = _body.front();

    if (_isGameOver)
        return {};

    if (_currentDirection == Arcade::InputCommand::UP)
        snakeHead.second--;
    if (_currentDirection == Arcade::InputCommand::DOWN)
        snakeHead.second++;
    if (_currentDirection == Arcade::InputCommand::LEFT)
        snakeHead.first--;
    if (_currentDirection == Arcade::InputCommand::RIGHT)
        snakeHead.first++;
    
    if (snakeHead.first < 0 || snakeHead.first >= _mapWidth ||
        snakeHead.second < 0 || snakeHead.second >= _mapHeigth) {
        _isGameOver = true;
        return {};
    }

    for (const auto& part : _body) {
        if (snakeHead == part) {
            _isGameOver = true;
            return {};
        }
    }

    _body.push_front(snakeHead);
    if (snakeHead == _apple) {
        _score += 1;
        spawnApple();
    } else {
        _body.pop_back();
    }
    return {};
}

std::vector<Arcade::SpriteObject> SnakeGame::getDisplayables() const
{
    std::vector<Arcade::SpriteObject> sprites;

    for (int x = -1; x <= _mapWidth; x++) {
        sprites.push_back({Arcade::ElementType::WALL, x, -1});
        sprites.push_back({Arcade::ElementType::WALL, x, _mapHeigth});
    }
    for (int y = 0; y < _mapHeigth; y++) {
        sprites.push_back({Arcade::ElementType::WALL, -1, y});
        sprites.push_back({Arcade::ElementType::WALL, _mapWidth, y});
    }
    sprites.push_back({Arcade::ElementType::FOOD, _apple.first, _apple.second});
    for (size_t i = 0; i < _body.size(); ++i) {
        Arcade::ElementType type = (i == 0) ? Arcade::ElementType::PLAYER_UP : Arcade::ElementType::PLAYER;
        sprites.push_back({type, _body[i].first, _body[i].second});
    }

    return sprites;
}

bool SnakeGame::isGameOver()
{
    return _isGameOver;
}

std::any SnakeGame::init() 
{ 
    return {}; 
}

void SnakeGame::handleMouse([[maybe_unused]] Arcade::MouseEvent inputs) 
{ 
}

extern "C" Arcade::IGames* createGame()
{
    return new SnakeGame();
}