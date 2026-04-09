/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_ncurses
*/

#include "./graphicals/arcade_ncurses.hpp"
#include "arcade_types.hpp"
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
    {'r', Arcade::InputCommand::RELOAD_GAME},
    {'R', Arcade::InputCommand::RELOAD_GAME},
    {'n', Arcade::InputCommand::SWITCH_GAME},
    {'N', Arcade::InputCommand::SWITCH_GAME},
    {'l', Arcade::InputCommand::SWITCH_LEADERBOARD},
    {'L', Arcade::InputCommand::SWITCH_LEADERBOARD},
    {27, Arcade::InputCommand::BACK},
    {'d', Arcade::InputCommand::SWITCH_DIFFICULTY},
    {'D', Arcade::InputCommand::SWITCH_DIFFICULTY},
    {' ', Arcade::InputCommand::FLAG},
    {'q', Arcade::InputCommand::QUIT},
    {'Q', Arcade::InputCommand::QUIT}
};

std::map<Arcade::ElementType, char> asciiMap = {
    {Arcade::ElementType::WALL, '#'},
    {Arcade::ElementType::BOMB, '@'},
    {Arcade::ElementType::FLAG, '>'},
    {Arcade::ElementType::PLAYER_UP, '^'},
    {Arcade::ElementType::PLAYER_DOWN, '^'},
    {Arcade::ElementType::PLAYER_LEFT, '^'},
    {Arcade::ElementType::PLAYER_RIGHT, '^'},
    {Arcade::ElementType::PLAYER_BODY, '0'},
    {Arcade::ElementType::PLAYER_TAIL, '~'},
    {Arcade::ElementType::PLAYER_TURN, '%'},
    {Arcade::ElementType::FOOD, '*'},
    {Arcade::ElementType::ONE, '1'},
    {Arcade::ElementType::TWO, '2'},
    {Arcade::ElementType::THREE, '3'},
    {Arcade::ElementType::FOUR, '4'},
    {Arcade::ElementType::FIVE, '5'},
    {Arcade::ElementType::SIX, '6'},
    {Arcade::ElementType::SEVEN, '7'},
    {Arcade::ElementType::EIGHT, '8'},
    {Arcade::ElementType::EMPTY, '-'}
};

/**
 * @brief Initializes ncurses mode, color pairs, and input settings.
 * @return Does not return a value.
 */
void NcursesAdapter::init(const std::unordered_map<Arcade::ElementType, std::string> &assets, std::string font)
{
    (void)font;
    (void)assets;
    if (_isInit)
        return;
    initscr();
    cbreak();
    noecho();
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
    init_pair(9, COLOR_WHITE, COLOR_GREEN);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    _isInit = true;
}

/**
 * @brief Clears the ncurses back buffer.
 * @return Does not return a value.
 */
void NcursesAdapter::clear()
{
    ::clear();
}

/**
 * @brief Draws one filled tile at the given grid coordinates.
 * @param tileX Tile X position.
 * @param tileY Tile Y position.
 * @param color Tile color.
 * @return Does not return a value.
 */
void NcursesAdapter::drawTile(int tileX, int tileY, Arcade::Color color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(tileY, tileX * 2, "  ");
    attroff(COLOR_PAIR(colorPair));
}

/**
 * @brief Draws text at the given position using ncurses color attributes.
 * @param posX Text X position in tiles.
 * @param posY Text Y position in rows.
 * @param text Text content to display.
 * @param color Text color.
 * @param fontPath Unused in ncurses backend.
 * @param fontSize Unused in ncurses backend.
 * @return Does not return a value.
 */
void NcursesAdapter::drawText(int posX, int posY, const std::string &text, Arcade::Color color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24)
{
    (void)fontSize;
    (void)fontPath;
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvprintw(posY, posX * 2, "%s", text.c_str());
    attroff(COLOR_PAIR(colorPair));
}

/**
 * @brief Estimates text width in tile units for layout logic.
 * @param text Text to measure.
 * @param fontPath Unused in ncurses backend.
 * @param fontSize Unused in ncurses backend.
 * @return Estimated width in tiles.
 */
int NcursesAdapter::measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize)
{
    (void)fontPath;
    (void)fontSize;
    return static_cast<int>((text.length() + 1) / 2);
}

/**
 * @brief Draws a rectangle border in tile coordinates.
 * @param startX Left X position.
 * @param startY Top Y position.
 * @param rectWidth Rectangle width in tiles.
 * @param rectHeight Rectangle height in tiles.
 * @param color Border color.
 * @return Does not return a value.
 */
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

/**
 * @brief Draws a simple circle marker around a center position.
 * @param centerX Circle center X.
 * @param centerY Circle center Y.
 * @param radius Circle radius in tiles.
 * @param color Circle color.
 * @return Does not return a value.
 */
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

/**
 * @brief Placeholder for sprite rendering in ncurses backend.
 * @param posX Sprite X position.
 * @param posY Sprite Y position.
 * @param spritePath Path to sprite resource.
 * @return Does not return a value.
 */
void NcursesAdapter::drawSprite(int posX, int posY, const std::string &spritePath)
{
    (void)posX;
    (void)posY;
    (void)spritePath;
}

/**
 * @brief Draws a single boxed tile with the requested color.
 * @param x Tile X position.
 * @param y Tile Y position.
 * @param color Box color.
 * @return Does not return a value.
 */
void NcursesAdapter::drawbox(int x, int y, Arcade::Color color)
{
    int colorPair = static_cast<int>(color) + 1;
    attron(COLOR_PAIR(colorPair));
    mvaddch(y, x * 2, '#');
    mvaddch(y, x * 2 + 1, '#');
    attroff(COLOR_PAIR(colorPair));
}

/**
 * @brief Draws a list of sprite objects using ASCII mappings.
 * @param objects Sprite objects to render.
 * @return Does not return a value.
 */
void NcursesAdapter::draw(const std::vector<Arcade::SpriteObject> &objects)
{
    for (const auto &obj : objects) {
        if (obj.type == Arcade::ElementType::WALL) {
            drawbox(obj.x, obj.y, Arcade::Color::WHITE);
            continue;
        }
        if (obj.type == Arcade::ElementType::PLAYER) {
            mvchgat(obj.y, obj.x * 2, 2, A_NORMAL, 9, NULL);
            continue;
        }
        if (obj.type == Arcade::ElementType::PLAYER_BODY || obj.type == Arcade::ElementType::PLAYER_TAIL) {
            mvchgat(obj.y, obj.x * 2, 2, A_NORMAL, 3, NULL); 
            const auto it = asciiMap.find(obj.type);
            if (it != asciiMap.end()) {
                mvaddch(obj.y, obj.x * 2, it->second);
                mvaddch(obj.y, obj.x * 2 + 1, it->second);
            }
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

/**
 * @brief Draws a complete display frame containing sprites and text.
 * @param data Aggregated display data to render.
 * @return Does not return a value.
 */
void NcursesAdapter::draw(const Arcade::DisplayData &data)
{
    draw(data.sprites);
    for (const auto &text : data.texts) {
        drawText(text.x, text.y, text.content, text.color, "", 24);
    }
}

/**
 * @brief Draws a line between two points using a Bresenham-like algorithm.
 * @param xStart Start X position.
 * @param yStart Start Y position.
 * @param xEnd End X position.
 * @param yEnd End Y position.
 * @param color Line color.
 * @return Does not return a value.
 */
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

/**
 * @brief Flushes the ncurses buffer to the terminal screen.
 * @return Does not return a value.
 */
void NcursesAdapter::display()
{
    refresh();
}

/**
 * @brief Polls keyboard and mouse events and maps them to Arcade commands.
 * @return Command vector containing detected input or NONE.
 */
std::vector<Arcade::InputCommand> NcursesAdapter::getKeyboardEvents()
{
    std::vector<Arcade::InputCommand> commands;
    int key = getch();

    if (key == ERR) {
        commands.push_back(Arcade::InputCommand::NONE);
        return commands;
    }

    if (key >= 32 && key <= 126 && key != '\n') {
        _textInput += static_cast<char>(key);
    }
    if (key == '\b' || key == KEY_BACKSPACE) {
        if (!_textInput.empty()) {
            _textInput.pop_back();
        }
    }

    const auto mapped = NcursesAdapter::KEY_MAP.find(key);
    if (mapped != NcursesAdapter::KEY_MAP.end()) {
        commands.push_back(mapped->second);
        if (mapped->second == Arcade::InputCommand::QUIT) {
            return commands;
        }
    }
    if (commands.empty()) {
        commands.push_back(Arcade::InputCommand::NONE);
    }
    return commands;
}

/**
 * @brief Returns the current terminal size converted to tile units.
 * @return Pair containing width and height in tiles.
 */
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

/**
 * @brief Gets the last text input from keyboard.
 * @return String containing typed characters and clears buffer.
 */
std::string NcursesAdapter::getTextInput()
{
    std::string result = _textInput;
    _textInput.clear();
    return result;
}

/**
 * @brief Restores terminal state by leaving ncurses mode.
 * @return Does not return a value.
 */
NcursesAdapter::~NcursesAdapter()
{
    endwin();
}

/**
 * @brief Returns the library type used by the loader.
 * @return 1 for a graphical library.
 */
extern "C" {
    int getType() {
        return 1;
    }

    /**
     * @brief Creates an ncurses graphical adapter for dynamic loading.
     * @return Pointer to a newly allocated IGraphic implementation.
     */
    Arcade::IGraphic* entryPoint() {
        return new NcursesAdapter();
    }
}
