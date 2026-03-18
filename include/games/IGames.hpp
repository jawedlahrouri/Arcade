/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igames
*/

#ifndef IGAMES_HPP_
#define IGAMES_HPP_

#include <string>
#include "graphicals/IGraphic.hpp"

class IGames {
    public:
        virtual ~IGames() = default;

        virtual void init(IGraphic& graphicLib) = 0;
        virtual void update() = 0;
        virtual void handleInput(const std::string& input) = 0;
        virtual void draw(IGraphic& graphicLib) = 0;
        virtual bool isGameOver() = 0;

    protected:
    private:
};

#endif /* !IGAMES_HPP_ */
