/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/
#include "games/arcade_snake.hpp"
#include <algorithm>
#include <random>
#include <chrono>

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
        _apple.first = (rand() % (_mapWidth - 1)) + 1; 
        _apple.second = (rand() % (_mapHeigth - 1)) + 1;
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
    static auto lastMove = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMove);

    if (elapsed.count() < 150) {
        return {}; 
    }
    lastMove = now;

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

Arcade::DisplayData SnakeGame::getDisplayables() const
{
    Arcade::DisplayData data;

    for (int x = 0; x <= _mapWidth; x++) {
        data.sprites.push_back({Arcade::ElementType::WALL, x, 0});
        data.sprites.push_back({Arcade::ElementType::WALL, x, _mapHeigth});
    }
    for (int y = 0; y < _mapHeigth; y++) {
        data.sprites.push_back({Arcade::ElementType::WALL, 0, y});
        data.sprites.push_back({Arcade::ElementType::WALL, _mapWidth, y});
    }
    data.sprites.push_back({Arcade::ElementType::FOOD, _apple.first, _apple.second});
    for (size_t i = 0; i < _body.size(); i++) {
        Arcade::ElementType type;
        if (i == 0) {
            if (_currentDirection == Arcade::InputCommand::UP)
                type = Arcade::ElementType::PLAYER_UP;
            else if (_currentDirection == Arcade::InputCommand::DOWN)
                type = Arcade::ElementType::PLAYER_DOWN;
            else if (_currentDirection == Arcade::InputCommand::LEFT)
                type = Arcade::ElementType::PLAYER_LEFT;
            else
                type = Arcade::ElementType::PLAYER_RIGHT;
        } else {
            type = Arcade::ElementType::PLAYER;
        }
        data.sprites.push_back({type, _body[i].first, _body[i].second});
    }

    data.texts.push_back({"SNAKE", 25, 0, Arcade::Color::WHITE});
    data.texts.push_back({"Score: " + std::to_string(_score), 25, 1, Arcade::Color::WHITE});

    return data;
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