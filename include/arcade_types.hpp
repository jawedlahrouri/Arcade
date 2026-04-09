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
        SWITCH_GAME,
        RELOAD_GAME,
        BACK,
        FLAG,
        QUIT,
        SWITCH_LEADERBOARD,
        SWITCH_DIFFICULTY
    };

    struct MouseEvent {
        int x;
        int y;
        bool leftClick;
        bool rightClick;
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
        PLAYER_TAIL,
        PLAYER_TURN,
        PLAYER_BODY,
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

    struct TextEntity {
        std::string content;
        int x;
        int y;
        bool isHighlighted = false;
        std::string fontPath = "";
        int fontSize = 24;
        Color color = Color::WHITE;

        TextEntity() = default;

        TextEntity(const std::string &content_, int x_, int y_, Color color_)
            : content(content_), x(x_), y(y_), color(color_)
        {}
    };


    struct DisplayData {
        std::vector<SpriteObject> sprites;
        std::vector<TextEntity> texts;
    };

    struct Snake {
        std::string name;
        std::pair<int, int> apple_coords;
        std::vector<std::pair<int, int>> coords_snake;
        int score;
    };

    struct Minesweeper {
        std::string name;
        std::vector<std::pair<int, int>> bombs_coords;
        std::vector<std::pair<int, int>> reveal_case;
        std::vector<std::pair<int, int>> hidden_case;
        std::vector<std::pair<int, int>> flag_case;
        std::vector<std::pair<int, std::pair<int, int>>> values_case;
        int difficulty;
        int flag_left;
    };
}

#endif /* !ARCADE_TYPES_HPP_ */