/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/
#include "games/arcade_snake.hpp"

void SnakeGame::init() {
    _isGameOver = false;
}

void SnakeGame::update() {
    // TODO: Update game logic here
}

void SnakeGame::handleInput(const std::vector<Arcade::InputCommand>& inputs) {
    for (const auto input : inputs) {
        if (input == Arcade::InputCommand::BACK || input == Arcade::InputCommand::QUIT) {
            _isGameOver = true;
            return;
        }
    }
    // TODO: Handle other inputs (movement, etc)
}

std::vector<Arcade::SpriteObject> SnakeGame::getDisplayables() const
{
    return {
        {Arcade::ElementType::WALL, 10, 5},
        {Arcade::ElementType::WALL, 11, 5},
        {Arcade::ElementType::WALL, 12, 5},
        {Arcade::ElementType::PLAYER_RIGHT, 11, 7},
        {Arcade::ElementType::FOOD, 15, 7}
    };
}

extern "C" IGames* createGame() {
    return new SnakeGame();
}