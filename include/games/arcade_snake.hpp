/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/

#ifndef ARCADE_SNAKE_HPP_
#define ARCADE_SNAKE_HPP_
#include "./games/IGames.hpp"
#include "./graphicals/IGraphic.hpp"

class SnakeGame : public IGames {
    public:
    SnakeGame() = default;
    ~SnakeGame() = default;

    void init(IGraphic &graphicLib) override;
    void update() override;
    void handleInput(const std::string& input) override;
    void draw(IGraphic& graphicLib) override;
    bool isGameOver() override { return _isGameOver; }

    private:
    bool _isGameOver = false;
};

#endif /* !ARCADE_SNAKE_HPP_ */
