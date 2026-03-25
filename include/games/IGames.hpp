/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igames
*/

#ifndef IGAMES_HPP_
#define IGAMES_HPP_

#include <vector>
#include "graphicals/IGraphic.hpp"

class IGames {
    public:
        virtual ~IGames() = default;

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void handleInput(const std::vector<Arcade::InputCommand>& inputs) = 0;
        virtual bool isGameOver() = 0;
        virtual std::vector<Arcade::SpriteObject> getDisplayables() const = 0;

    protected:
    private:
};

#endif /* !IGAMES_HPP_ */
