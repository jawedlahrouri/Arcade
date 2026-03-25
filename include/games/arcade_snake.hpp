/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/

#ifndef ARCADE_SNAKE_HPP_
#define ARCADE_SNAKE_HPP_
#include "./games/IGames.hpp"
#include <deque>
#include <map>

class SnakeGame : public Arcade::IGames {
    private:
        int _score;
        int _mapWidth;
        int _mapHeigth;
        std::deque<std::pair<int, int>> _body;
        std::pair<int, int> _apple;
        Arcade::InputCommand _currentDirection;
        bool _isGameOver;
        void spawnApple();

    public:
        SnakeGame();
        ~SnakeGame() override;
        std::any update() override;
        void handleKey(const std::vector<Arcade::InputCommand>& inputs) override;
        int getScore() override;
        bool isGameOver() override;
        std::vector<Arcade::SpriteObject> getDisplayables() const override; // Changé GameObject -> SpriteObject
};

#endif /* !ARCADE_SNAKE_HPP_ */
