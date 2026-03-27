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
#include <iostream>

namespace Arcade {
    enum class InputCommand {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        ENTER,
        SWITCH_GRAPHICS,
        SWITCH_GAME,
        RELOAD_GAME,
        BACK,
        FLAG,
        QUIT,
        SWITCH_LEADERBOARD
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
        BOMB,
        FLAG,
        SCORE,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT
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

    struct MouseEvent {
        int x;
        int y;
        bool isClicked;
    };

    struct Snake {
        std::string name;
        std::pair<int, int> apple_coords;
        std::vector<std::pair<int, int>> coords_snake;
        int score;
    };
}

#endif /* !ARCADE_TYPES_HPP_ */