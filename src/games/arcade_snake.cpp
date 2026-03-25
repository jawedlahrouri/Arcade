/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/
#include "games/arcade_snake.hpp"

/**
 * @brief Initializes the snake game state before the loop starts.
 * @return Does not return a value.
 */
void SnakeGame::init() {
    _isGameOver = false;
}

/**
 * @brief Updates one simulation step of the snake game.
 * @return Does not return a value.
 */
void SnakeGame::update() {
    // TODO: Update game logic here
}

/**
 * @brief Processes player commands and updates immediate game state.
 * @param inputs List of commands polled for the current frame.
 * @return Does not return a value.
 */
void SnakeGame::handleInput(const std::vector<Arcade::InputCommand>& inputs) {
    for (const auto input : inputs) {
        if (input == Arcade::InputCommand::BACK || input == Arcade::InputCommand::QUIT) {
            _isGameOver = true;
            return;
        }
    }
    // TODO: Handle other inputs (movement, etc)
}

/**
 * @brief Builds the sprite list representing the current frame.
 * @return Vector of sprite objects to render.
 */
Arcade::DisplayData SnakeGame::getDisplayables() const
{
    Arcade::DisplayData data;
    data.sprites = {
        {Arcade::ElementType::WALL, 10, 5},
        {Arcade::ElementType::WALL, 11, 5},
        {Arcade::ElementType::WALL, 12, 5},
        {Arcade::ElementType::PLAYER_RIGHT, 11, 7},
        {Arcade::ElementType::FOOD, 15, 7}
    };
    return data;
}

/**
 * @brief Creates a snake game instance for dynamic loading.
 * @return Pointer to a newly allocated IGames implementation.
 */
extern "C" IGames* createGame() {
    return new SnakeGame();
}