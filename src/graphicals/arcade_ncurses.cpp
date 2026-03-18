/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#include "./graphicals/arcade_ncurses.hpp"
#include "exception.hpp"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

void NcursesAdapter::init()
{
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
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

void NcursesAdapter::drawTile(int tileX, int tileY, ArcadeColor color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(tileY, tileX * 2, "  ");
    attroff(COLOR_PAIR(colorPair));
}
// default path: "asset/font/coolvetica/Coolvetica Rg.otf"
// defult size: 24
void NcursesAdapter::drawText(int posX, int posY, const std::string &text, ArcadeColor color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(posY, posX * 2, "%s", text.c_str());
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::drawRect(int startX, int startY, int rectWidth, int rectHeight, ArcadeColor color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    for (int col = startX; col < startX + rectWidth; ++col) {
        mvaddch(startY, col * 2, '#');
        mvaddch(startY + rectHeight - 1, col * 2, '#');
    }
    for (int row = startY; row < startY + rectHeight; ++row) {
        mvaddch(row, startX * 2, '#');
        mvaddch(row, (startX + rectWidth - 1) * 2, '#');
    }
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::drawCircle(int centerX, int centerY, int radius, ArcadeColor color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvaddch(centerY, centerX * 2, 'o');
    mvaddch(centerY - radius, centerX * 2, 'o');
    mvaddch(centerY + radius, centerX * 2, 'o');
    mvaddch(centerY, (centerX - radius) * 2, 'o');
    mvaddch(centerY, (centerX + radius) * 2, 'o');
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::drawSprite(int posX, int posY, const std::string &spritePath)
{
    (void)posX; (void)posY; (void)spritePath;
    //TODO: A VOIR COMMENT IMPLEMENTER CA EN MODE ASCII ART LES SPRITES
}

void NcursesAdapter::drawLine(int xStart, int yStart, int xEnd, int yEnd, ArcadeColor color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));

    int x = xStart;
    int y = yStart;
    int deltaX = std::abs(xEnd - xStart);
    int deltaY = std::abs(yEnd - yStart);
    int stepX = (xStart < xEnd) ? 1 : -1;
    int stepY = (yStart < yEnd) ? 1 : -1;
    int errorTerm = deltaX - deltaY;

    while (true) {
        int screenCol = x * 2;
        int screenRow = y;
        mvaddch(screenRow, screenCol, '*');
        mvaddch(screenRow, screenCol + 1, '*');

        if (x == xEnd && y == yEnd) break;

        int error2 = 2 * errorTerm;
        if (error2 > -deltaY) { errorTerm -= deltaY; x += stepX; }
        if (error2 < deltaX)  { errorTerm += deltaX; y += stepY; }
    }
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::display()
{
    refresh();
}

std::string NcursesAdapter::handleEvents()
{
    int key = getch();
    if (key == 'q' || key == 'Q') {
        return "QUIT";
    }
    if (key == KEY_UP) return "UP";
    if (key == KEY_DOWN) return "DOWN";
    if (key == KEY_LEFT) return "LEFT";
    if (key == KEY_RIGHT) return "RIGHT";
    if (key == '\n' || key == KEY_ENTER) return "ENTER";

    if (key == KEY_MOUSE) {
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
    int termRows, termCols;
    getmaxyx(stdscr, termRows, termCols);
    std::filesystem::path logPath = "logs/arcade_ncurses.log";
    std::ofstream log(logPath, std::ios::app);
    if (log.is_open()) {
        log << "Screen size: " << termCols << "x" << termRows << std::endl;
        log.close();
    }
    // width in "tiles" - each tile uses 2 columns
    return { termCols / 2, termRows };
}

NcursesAdapter::~NcursesAdapter()
{
    endwin();
}

extern "C" IGraphic* createGraphical() {
    return new NcursesAdapter();
}
