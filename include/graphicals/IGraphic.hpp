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

    // Colors
    enum class Color {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE
    };

    // elements
    enum class ElementType {
        EMPTY,
        WALL,
        PLAYER_UP,
        PLAYER_DOWN,
        PLAYER_LEFT,
        PLAYER_RIGHT,
        FOOD,
        SCORE
    };

    struct GameObject {
        ElementType type;
        int x;
        int y;
    };
}

// all the functions that the graphical library must implement
class IGraphic {
    public:
        virtual ~IGraphic() = default;

        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize = 24) = 0;
        virtual int measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize = 24) = 0;
        virtual void drawRect(int x, int y, int width, int height, Arcade::Color color) = 0;
        virtual void drawCircle(int x, int y, int radius, Arcade::Color color) = 0;
        virtual void drawSprite(int x, int y, const std::string &spritePath) = 0;
        virtual void drawLine(int x1, int y1, int x2, int y2, Arcade::Color color) = 0;
        virtual void drawTile(int x, int y, Arcade::Color color) = 0;
        virtual void drawbox(int x, int y, Arcade::Color color) = 0;
        virtual void draw(const std::vector<Arcade::GameObject> &objects) = 0;
        virtual std::vector<Arcade::InputCommand> pollInputs() = 0;
        virtual std::pair<int,int> getScreenSize() = 0;

    protected:
    private:
};

#endif /* !IGRAPHIC_HPP_ */
