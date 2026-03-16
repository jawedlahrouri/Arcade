/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#include "./graphicals/arcade_ncurses.hpp"
#include "exception.hpp"
#include <iostream>
#include <ostream>
#include <cstdlib>

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

void NcursesAdapter::drawText(int x, int y, const std::string &text, ArcadeColor color)
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
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            mvprintw(y + j, (x + i) * 2, "  ");
        }
    }
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::drawCircle(int x, int y, int radius, ArcadeColor color)
{
    int cx = x;
    int cy = y;
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx*dx + dy*dy <= radius*radius) {
                int px = cx + dx;
                int py = cy + dy;
                mvprintw(py, px * 2, "  ");
            }
        }
    }
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::drawSprite(int x, int y, const std::string &spritePath)
{
    (void)x; (void)y; (void)spritePath;
    // Not implemented for ncurses adapter.
}

void NcursesAdapter::drawLine(int x1, int y1, int x2, int y2, ArcadeColor color)
{
    int dx = std::abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    int e2;
    int cx = x1;
    int cy = y1;
    int color_pair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(color_pair));
    while (true) {
        mvprintw(cy, cx * 2, "  ");
        if (cx == x2 && cy == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            cx += sx;
        }
        if (e2 <= dx) {
            err += dx;
            cy += sy;
        }
    }
    attroff(COLOR_PAIR(color_pair));
}

void NcursesAdapter::display()
{
    refresh();
}

void NcursesAdapter::handleEvents()
{
    int ch = getch();
    if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON1_CLICKED) {
                std::cout << "Mouse event at (" << event.x << ", " << event.y << ")" << std::endl;
            }
        }
    }
}

NcursesAdapter::~NcursesAdapter()
{
    endwin();
}

extern "C" IGraphic* createGraphical() {
    return new NcursesAdapter();
}

//extern "C" {
//    IGraphic* create_graphic() {
//        return new NcursesAdapter();
//    }
//}