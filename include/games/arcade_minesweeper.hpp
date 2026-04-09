/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper
*/

#ifndef ARCADE_1_ARCADE_MINESWEEPER_HPP
#define ARCADE_1_ARCADE_MINESWEEPER_HPP

#include "IGames.hpp"
#include <chrono>

enum CellType {
    EMPTY = 0,
    BOMB = -1,
};

class arcade_minesweeper : public Arcade::IGames {
    private:
        int _width;
        int _height;
        int _difficulty = 1; 
        int _totalBombs;
        int _flagsPlaced;
        bool _isGameOver;
        bool _hasWon;
        bool _firstClick;
        std::chrono::steady_clock::time_point _startTime;
        int _timeLimit;
        int _remainingTime;
        std::vector<std::vector<int>> _grid;
        std::vector<std::vector<bool>> _revealed;
        std::vector<std::vector<bool>> _flagged;
        int _cursorX;
        int _cursorY;

    public:
        arcade_minesweeper() = default;
        ~arcade_minesweeper() override = default;

        std::any init() override;
        std::any update() override;
        void handleKey(const std::vector<Arcade::InputCommand>& inputs) override;
        void handleMouse(Arcade::MouseEvent inputs) override;
        int getScore() override;
        bool isGameOver() override;
        Arcade::DisplayData getDisplayables() const override;
        std::unordered_map<Arcade::ElementType, std::string> getAssets() override;
        void generate_bomb_field(int safe_x, int safe_y); 
        void reveal_case(int coord_x, int coord_y); 
        void reveal_neighbor_case(int coord_x, int coord_y);
        bool is_bomb(int coord_x, int coord_y);
        void splode_bomb();
        void end_game();
};

#endif //ARCADE_1_ARCADE_MINESWEEPER_HPP