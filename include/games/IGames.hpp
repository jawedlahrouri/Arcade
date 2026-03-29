/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igames
*/

#ifndef IGAMES_HPP_
#define IGAMES_HPP_
#
#include <vector>
#include <any>
#include "arcade_types.hpp"

namespace Arcade {
    class IGames {
    protected:
    private:

    public:
        virtual ~IGames() = default;

        virtual std::any init() = 0;
        virtual std::any update() = 0;
        virtual void handleKey(const std::vector<Arcade::InputCommand>& inputs) = 0;
        virtual void handleMouse(struct MouseEvent inputs) = 0;
        virtual int getScore() = 0;
        virtual bool isGameOver() = 0;
        virtual Arcade::DisplayData getDisplayables() const = 0;
    };

    struct Minesweeper {
        std::string name;
        std::vector<std::pair<int, int>> bombs_coords;
        std::vector<std::pair<int, int>> reveal_case;
        std::vector<std::pair<int, int>> hidden_case;
        std::vector<std::pair<int, int>> flag_case;
        std::vector<std::pair<int,std::pair <int, int>>> values_case;
        int difficulty;
        int flag_left;
    };
}

#endif /* !IGAMES_HPP_ */
