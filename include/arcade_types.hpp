/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_types
*/

#ifndef ARCADE_TYPES_HPP_
#define ARCADE_TYPES_HPP_

#include <string>
#include <vector>

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

    enum class ElementType {
        EMPTY,
        WALL,
        PLAYER,
        ENEMY,
        PLAYER_UP,
        PLAYER_DOWN,
        PLAYER_LEFT,
        PLAYER_RIGHT,
        FOOD,
        SCORE
    };

    struct SpriteObject {
        ElementType type;
        int x;
        int y;
    };

    struct TextObject {
        std::string text;
        int x;
        int y;
        Color color;
    };

    struct DisplayData {
        std::vector<SpriteObject> sprites;
        std::vector<TextObject> texts;
    };
}

#endif /* !ARCADE_TYPES_HPP_ */