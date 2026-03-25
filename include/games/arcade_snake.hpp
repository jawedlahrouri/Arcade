/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/

#ifndef ARCADE_SNAKE_HPP_
#define ARCADE_SNAKE_HPP_
#include "./games/IGames.hpp"

class SnakeGame : public IGames {
    public:
    /**
     * @brief Builds the snake game instance.
     * @return Does not return a value.
     */
    SnakeGame() = default;
    ~SnakeGame() = default;

    /**
     * @brief Initializes the snake game state.
     * @return Does not return a value.
     */
    void init() override;
    /**
     * @brief Updates one frame of game logic.
     * @return Does not return a value.
     */
    void update() override;
    /**
     * @brief Handles input commands affecting the snake game.
     * @param inputs Commands received for this frame.
     * @return Does not return a value.
     */
    void handleInput(const std::vector<Arcade::InputCommand>& inputs) override;
    /**
     * @brief Indicates whether the current game session is over.
     * @return true when game over was reached.
     */
    bool isGameOver() override { return _isGameOver; }
    /**
     * @brief Returns the list of sprites to render.
     * @return Vector of sprite objects for the current frame.
     */
    std::vector<Arcade::SpriteObject> getDisplayables() const override;

    private:
    bool _isGameOver = false;
};

#endif /* !ARCADE_SNAKE_HPP_ */
