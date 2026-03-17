/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igraphic
*/

#ifndef IGRAPHIC_HPP_
#define IGRAPHIC_HPP_
#include <string>
#include "./games/IEvents.hpp"

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
    WINDOW_CLOSE,
    UNKNOW
};

class ArcadeEvent : public IEvents {
    public:
        ArcadeEvent(ArcadeEventType type) : type(type) {}
        ArcadeEventType getType() const { return type; }

    private:
        ArcadeEventType type;
};

static ArcadeEvent KEY_PRESS_EVENT(ArcadeEventType::KEY_PRESS);
static ArcadeEvent KEY_RELEASE_EVENT(ArcadeEventType::KEY_RELEASE);
static ArcadeEvent MOUSE_MOVE_EVENT(ArcadeEventType::MOUSE_MOVE);
static ArcadeEvent MOUSE_BUTTON_PRESS_EVENT(ArcadeEventType::MOUSE_BUTTON_PRESS);
static ArcadeEvent MOUSE_BUTTON_RELEASE_EVENT(ArcadeEventType::MOUSE_BUTTON_RELEASE);
static ArcadeEvent WINDOW_CLOSE_EVENT(ArcadeEventType::WINDOW_CLOSE);

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

class IAssetLoader {
    public:
        virtual ~IAssetLoader() = default;

        virtual void loadTexture(const std::string &path) = 0;
        virtual void loadAudio(const std::string &path) = 0;
};

#endif /* !IGRAPHIC_HPP_ */
