/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igraphic
*/

#ifndef IGRAPHIC_HPP_
#define IGRAPHIC_HPP_
#include <string>

// Keys
enum class ArcadeKey {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ENTER,
    BACKSPACE,
    SPACE,
    ESCAPE,
    UNKNOWN
};

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

// Event types
enum class ArcadeEventType {
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_MOVE,
    MOUSE_BUTTON_PRESS,
    MOUSE_BUTTON_RELEASE,
    WINDOW_CLOSE
};

// Mouse buttons
enum class ArcadeMouseButton {
    LEFT,
    RIGHT,
    MIDDLE,
    UNKNOWN
};

class IGraphic {
    public:
        virtual ~IGraphic() = default;

        virtual void init() = 0;
        virtual void clear() = 0;
        virtual void display() = 0;
        virtual void drawText(int x, int y, const std::string &text, ArcadeColor color) = 0;
        virtual void drawRect(int x, int y, int width, int height, ArcadeColor color) = 0;
        virtual void drawCircle(int x, int y, int radius, ArcadeColor color) = 0;
        virtual void drawSprite(int x, int y, const std::string &spritePath) = 0;
        virtual void drawLine(int x1, int y1, int x2, int y2, ArcadeColor color) = 0;
        virtual void drawTile(int x, int y, ArcadeColor color) = 0;
        virtual void handleEvents() = 0;

    protected:
    private:
};

#endif /* !IGRAPHIC_HPP_ */
