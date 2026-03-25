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
    SnakeGame() = default;
    ~SnakeGame() = default;

    void init() override;
    void update() override;
    void handleInput(const std::vector<Arcade::InputCommand>& inputs) override;
    bool isGameOver() override { return _isGameOver; }
    std::vector<Arcade::SpriteObject> getDisplayables() const override;

    private:
    bool _isGameOver = false;
};

#endif /* !ARCADE_SNAKE_HPP_ */
