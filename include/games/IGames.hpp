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
}

#endif /* !IGAMES_HPP_ */
