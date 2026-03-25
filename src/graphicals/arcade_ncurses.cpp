/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#include "./graphicals/arcade_ncurses.hpp"
#include <filesystem>
#include <iostream>
#include <ostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <map>

// TODO: Peut être déplaacer les touches spécifique au jeu.
// Par exemple UP pour snake est dans snake.cpp
//SWITCH GRAPHICS et BACK sont dans la lib graphique selon moi (TBD)
const std::unordered_map<int, Arcade::InputCommand> NcursesAdapter::KEY_MAP = {
    {KEY_UP, Arcade::InputCommand::UP},
    {KEY_DOWN, Arcade::InputCommand::DOWN},
    {KEY_LEFT, Arcade::InputCommand::LEFT},
    {KEY_RIGHT, Arcade::InputCommand::RIGHT},
    {'\n', Arcade::InputCommand::ENTER},
    {KEY_ENTER, Arcade::InputCommand::ENTER},
    {'s', Arcade::InputCommand::SWITCH_GRAPHICS},
    {'S', Arcade::InputCommand::SWITCH_GRAPHICS},
    {27, Arcade::InputCommand::BACK},
    {'q', Arcade::InputCommand::QUIT},
    {'Q', Arcade::InputCommand::QUIT}
};

std::map<Arcade::ElementType, char> asciiMap = {
    {Arcade::ElementType::WALL, '#'},
    {Arcade::ElementType::PLAYER_UP, '^'},
    {Arcade::ElementType::FOOD, '*'}
};

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

void NcursesAdapter::drawTile(int tileX, int tileY, Arcade::Color color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(tileY, tileX * 2, "  ");
    attroff(COLOR_PAIR(colorPair));
}
// default path: "asset/font/coolvetica/Coolvetica Rg.otf"
// defult size: 24
void NcursesAdapter::drawText(int posX, int posY, const std::string &text, Arcade::Color color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24)
{
    (void)fontSize;
    (void)fontPath;
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(posY, posX * 2, "%s", text.c_str());
    attroff(COLOR_PAIR(colorPair));
}

int NcursesAdapter::measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize)
{
    (void)fontPath;
    (void)fontSize;
    return static_cast<int>((text.length() + 1) / 2);
}

void NcursesAdapter::drawRect(int startX, int startY, int rectWidth, int rectHeight, Arcade::Color color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    for (int col = startX; col < startX + rectWidth; ++col) {
        mvprintw(startY, col * 2, "##");
        mvprintw(startY + rectHeight - 1, col * 2, "##");
    }
    for (int row = startY; row < startY + rectHeight; ++row) {
        mvprintw(row, startX * 2, "##");
        mvprintw(row, (startX + rectWidth - 1) * 2, "##");
    }
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::drawCircle(int centerX, int centerY, int radius, Arcade::Color color)
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
    (void)posX;
    (void)posY;
    (void)spritePath;
}

void NcursesAdapter::drawbox(int x, int y, Arcade::Color color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvaddch(y, x * 2, '#');
    mvaddch(y, x * 2 + 1, '#');
    attroff(COLOR_PAIR(colorPair));
}

void NcursesAdapter::draw(const std::vector<Arcade::SpriteObject> &objects)
{
    for (const auto &obj : objects) {
        if (obj.type == Arcade::ElementType::WALL) {
            drawbox(obj.x, obj.y, Arcade::Color::WHITE);
            continue;
        }
        const auto it = asciiMap.find(obj.type);
        if (it == asciiMap.end()) {
            continue;
        }
        const int screenCol = obj.x * 2;
        mvaddch(obj.y, screenCol, it->second);
        mvaddch(obj.y, screenCol + 1, it->second);
    }
}

void NcursesAdapter::draw(const Arcade::DisplayData &data)
{
    draw(data.sprites);
    for (const auto &text : data.texts) {
        drawText(text.x, text.y, text.text, text.color, "", 24);
    }
}

void NcursesAdapter::drawLine(int xStart, int yStart, int xEnd, int yEnd, Arcade::Color color)
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

std::vector<Arcade::InputCommand> NcursesAdapter::pollInputs()
{
    std::vector<Arcade::InputCommand> commands;
    int key = getch();

    if (key == ERR) {
        commands.push_back(Arcade::InputCommand::NONE);
        return commands;
    }

    const auto mapped = NcursesAdapter::KEY_MAP.find(key);
    if (mapped != NcursesAdapter::KEY_MAP.end()) {
        commands.push_back(mapped->second);
        if (mapped->second == Arcade::InputCommand::QUIT) {
            return commands;
        }
    }

    if (key == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON1_CLICKED) {
                std::cout << "Mouse event at (" << event.x << ", " << event.y << ")" << std::endl;
            }
        }
    }

    if (commands.empty()) {
        commands.push_back(Arcade::InputCommand::NONE);
    }
    return commands;
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
    return { termCols / 2, termRows };
}

NcursesAdapter::~NcursesAdapter()
{
    endwin();
}

extern "C" IGraphic* createGraphical() {
    return new NcursesAdapter();
}
