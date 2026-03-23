/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igraphic
*/

#ifndef IGRAPHIC_HPP_
#define IGRAPHIC_HPP_
#include <string>
#include <vector>
#include <utility>

namespace Arcade {
enum class InputCommand {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    SWITCH_GRAPHICS,
    BACK,
    QUIT
};
}

// Colors
enum class ArcadeColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

// all the functions that the graphical library must implement
class IGraphic {
    public:
        virtual ~IGraphic() = default;

        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) = 0;
        virtual void drawRect(int x, int y, int width, int height, ArcadeColor color) = 0;
        virtual void drawCircle(int x, int y, int radius, ArcadeColor color) = 0;
        virtual void drawSprite(int x, int y, const std::string &spritePath) = 0;
        virtual void drawLine(int x1, int y1, int x2, int y2, ArcadeColor color) = 0;
        virtual void drawTile(int x, int y, ArcadeColor color) = 0;
        virtual std::vector<Arcade::InputCommand> pollInputs() = 0;
        virtual std::pair<int,int> getScreenSize() = 0;

    protected:
    private:
};

#endif /* !IGRAPHIC_HPP_ */
