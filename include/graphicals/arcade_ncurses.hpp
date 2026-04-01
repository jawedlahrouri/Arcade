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

class NcursesAdapter : public Arcade::IGraphic {
private:
    static const std::unordered_map<int, Arcade::InputCommand> KEY_MAP;

public:
    /**
     * @brief Initializes the ncurses graphical backend.
     * @return Does not return a value.
     */
    void init() override;
    /**
     * @brief Clears the terminal render buffer.
     * @return Does not return a value.
     */
    void clear() override;
    /**
     * @brief Draws a filled tile at the requested coordinates.
     * @param x Tile X position.
     * @param y Tile Y position.
     * @param color Tile color.
     * @return Does not return a value.
     */
    void drawTile(int x, int y, Arcade::Color color);
    /**
     * @brief Draws text using ncurses primitives.
     * @param x Text X position.
     * @param y Text Y position.
     * @param text Text content.
     * @param color Text color.
     * @param fontPath Font path parameter (unused in ncurses).
     * @param fontSize Font size parameter (unused in ncurses).
     * @return Does not return a value.
     */
    void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize);
    /**
     * @brief Measures text width in ncurses logical units.
     * @param text Text content.
     * @param fontPath Font path parameter (unused in ncurses).
     * @param fontSize Font size parameter (unused in ncurses).
     * @return Measured text width.
     */
    int measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize);
    /**
     * @brief Draws a rectangle border.
     * @param x Rectangle X position.
     * @param y Rectangle Y position.
     * @param width Rectangle width.
     * @param height Rectangle height.
     * @param color Rectangle color.
     * @return Does not return a value.
     */
    void drawRect(int x, int y, int width, int height, Arcade::Color color);
    /**
     * @brief Draws a circle.
     * @param x Circle center X position.
     * @param y Circle center Y position.
     * @param radius Circle radius.
     * @param color Circle color.
     * @return Does not return a value.
     */
    void drawCircle(int x, int y, int radius, Arcade::Color color);
    /**
     * @brief Draws a sprite from a path.
     * @param x Sprite X position.
     * @param y Sprite Y position.
     * @param spritePath Path to sprite resource.
     * @return Does not return a value.
     */
    void drawSprite(int x, int y, const std::string &spritePath);
    /**
     * @brief Draws a line between two points.
     * @param x1 Start X position.
     * @param y1 Start Y position.
     * @param x2 End X position.
     * @param y2 End Y position.
     * @param color Line color.
     * @return Does not return a value.
     */
    void drawLine(int x1, int y1, int x2, int y2, Arcade::Color color);
    /**
     * @brief Draws one box tile at the given location.
     * @param x Tile X position.
     * @param y Tile Y position.
     * @param color Tile color.
     * @return Does not return a value.
     */
    void drawbox(int x, int y, Arcade::Color color);
    /**
     * @brief Draws a list of sprite objects.
     * @param objects Sprite objects to draw.
     * @return Does not return a value.
     */
    void draw(const std::vector<Arcade::SpriteObject> &objects);
    /**
     * @brief Draws complete display data.
     * @param data Display data to render.
     * @return Does not return a value.
     */
    void draw(const Arcade::DisplayData &data) override;
    /**
     * @brief Flushes the render buffer to the terminal.
     * @return Does not return a value.
     */
    void display() override;
    /**
     * @brief Polls inputs and maps them to Arcade commands.
     * @return Vector of input commands.
     */
    std::vector<Arcade::InputCommand> getKeyboardEvents() override;
    /**
     * @brief Gets the last text input from keyboard.
     * @return String containing typed characters.
     */
    std::string getTextInput() override;
    /**
     * @brief Returns the current terminal size in logical units.
     * @return Pair of width and height.
     */
    std::pair<int,int> getScreenSize() override;
    /**
     * @brief Releases ncurses resources.
     * @return Does not return a value.
     */
    ~NcursesAdapter() override;

private:
    std::string _textInput;
};

#endif /* !ARCADE_NCURSES_HPP_ */
