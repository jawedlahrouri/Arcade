/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#ifndef ARCADE_NCURSES_HPP_
#define ARCADE_NCURSES_HPP_
#include "IGraphic.hpp"
#include <ncurses.h>
#include <unordered_map>

class NcursesAdapter : public IGraphic {
private:
    static const std::unordered_map<int, Arcade::InputCommand> KEY_MAP;

public:
    void init() override;
    void clear() override;
    void drawTile(int x, int y, ArcadeColor color) override;
    void drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath, int fontSize) override;
    void drawRect(int x, int y, int width, int height, ArcadeColor color) override;
    void drawCircle(int x, int y, int radius, ArcadeColor color) override;
    void drawSprite(int x, int y, const std::string &spritePath) override;
    void drawLine(int x1, int y1, int x2, int y2, ArcadeColor color) override;
    void display() override;
    std::vector<Arcade::InputCommand> pollInputs() override;
    std::pair<int,int> getScreenSize() override;
    ~NcursesAdapter() override;
};

#endif /* !ARCADE_NCURSES_HPP_ */
