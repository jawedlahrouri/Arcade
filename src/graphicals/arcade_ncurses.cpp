/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#include "./graphicals/arcade_ncurses.hpp"
#include "exception.hpp"
#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <cstdlib>
#include <cmath>

void NcursesAdapter::init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(7, COLOR_CYAN, COLOR_CYAN);
    init_pair(8, COLOR_WHITE, COLOR_WHITE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
}

void NcursesAdapter::clear()
{
    ::clear();
}

void NcursesAdapter::drawTile(int x, int y, ArcadeColor color)
{
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x * 2, "  ");
    attroff(COLOR_PAIR(color_pair));
}
// default path: "asset/font/coolvetica/Coolvetica Rg.otf"
// defult size: 24
void NcursesAdapter::drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24)
{
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    mvprintw(y, x * 2, "%s", text.c_str());
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::drawRect(int x, int y, int width, int height, ArcadeColor color)
{
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    for (int i = x; i < x + width; ++i) {
        mvaddch(y, i * 2, '#');
        mvaddch(y + height - 1, i * 2, '#');
    }
    for (int j = y; j < y + height; ++j) {
        mvaddch(j, x * 2, '#');
        mvaddch(j, (x + width - 1) * 2, '#');
    }
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::drawCircle(int x, int y, int radius, ArcadeColor color)
{
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    mvaddch(y, x * 2, 'o');
    mvaddch(y - radius, x * 2, 'o');
    mvaddch(y + radius, x * 2, 'o');
    mvaddch(y, (x - radius) * 2, 'o');
    mvaddch(y, (x + radius) * 2, 'o');
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::drawSprite(int x, int y, const std::string &spritePath)
{
    (void)x; (void)y; (void)spritePath;
    // A VOIR COMMENT IMPLEMENTER CA EN MODE ASCII ART LES SPRITES
}

void NcursesAdapter::drawLine(int x1, int y1, int x2, int y2, ArcadeColor color)
{
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));

    int x = x1;
    int y = y1;
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        int termX = x * 2;
        int termY = y;
        mvaddch(termY, termX, '*');
        mvaddch(termY, termX + 1, '*');

        if (x == x2 && y == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx)  { err += dx; y += sy; }
    }

    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::display()
{
    refresh();
}

std::string NcursesAdapter::handleEvents()
{
    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        return "QUIT";
    }
    if (ch == KEY_UP) return "UP";
    if (ch == KEY_DOWN) return "DOWN";
    if (ch == KEY_LEFT) return "LEFT";
    if (ch == KEY_RIGHT) return "RIGHT";
    if (ch == '\n' || ch == KEY_ENTER) return "ENTER";

    if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON1_CLICKED) {
                std::cout << "Mouse event at (" << event.x << ", " << event.y << ")" << std::endl;
            }
        }
    }
    return "NONE";
}

std::pair<int,int> NcursesAdapter::getScreenSize()
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    // width in "tiles" - each tile uses 2 columns
    return { cols / 2, rows };
}

NcursesAdapter::~NcursesAdapter()
{
    endwin();
}

extern "C" IGraphic* createGraphical() {
    return new NcursesAdapter();
}
