/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/

#ifndef ARCADE_SNAKE_HPP_
#define ARCADE_SNAKE_HPP_
#include "IGames.hpp"
#include <deque>
#include <map>
#include <string>
#include <unordered_map>

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
        void handleMouse(Arcade::MouseEvent inputs) override; 
        int getScore() override;
        bool isGameOver() override;
        Arcade::DisplayData getDisplayables() const override;
        std::unordered_map<Arcade::ElementType, std::string> getAssets() override;
        std::any init() override;
};

#endif /* !ARCADE_SNAKE_HPP_ */
