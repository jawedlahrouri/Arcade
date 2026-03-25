/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igraphic
*/

#ifndef IGRAPHIC_HPP_
#define IGRAPHIC_HPP_
#include "arcade_types.hpp"
#include <string>
#include <utility>

// all the functions that the graphical library must implement
class IGraphic {
    public:
        virtual ~IGraphic() = default;

        /**
         * @brief Initializes graphic backend resources.
         * @return Does not return a value.
         */
        virtual void init() = 0;
        /**
         * @brief Clears the current render target.
         * @return Does not return a value.
         */
        virtual void clear() = 0;
        /**
         * @brief Presents the current frame to the display.
         * @return Does not return a value.
         */
        virtual void display() = 0;
        /**
         * @brief Draws text using the backend font system.
         * @param x Text X position.
         * @param y Text Y position.
         * @param text Content to draw.
         * @param color Text color.
         * @param fontPath Path to font resource.
         * @param fontSize Font size in pixels.
         * @return Does not return a value.
         */
        virtual void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize = 24) = 0;
        /**
         * @brief Measures text width in backend logical units.
         * @param text Content to measure.
         * @param fontPath Path to font resource.
         * @param fontSize Font size in pixels.
         * @return Width of the text in logical units.
         */
        virtual int measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize = 24) = 0;
        /**
         * @brief Draws a rectangle border.
         * @param x Rectangle left X position.
         * @param y Rectangle top Y position.
         * @param width Rectangle width.
         * @param height Rectangle height.
         * @param color Rectangle color.
         * @return Does not return a value.
         */
        virtual void drawRect(int x, int y, int width, int height, Arcade::Color color) = 0;
        /**
         * @brief Draws a circle.
         * @param x Circle center X position.
         * @param y Circle center Y position.
         * @param radius Circle radius.
         * @param color Circle color.
         * @return Does not return a value.
         */
        virtual void drawCircle(int x, int y, int radius, Arcade::Color color) = 0;
        /**
         * @brief Draws a sprite from a resource path.
         * @param x Sprite X position.
         * @param y Sprite Y position.
         * @param spritePath Path to sprite resource.
         * @return Does not return a value.
         */
        virtual void drawSprite(int x, int y, const std::string &spritePath) = 0;
        /**
         * @brief Draws a line between two points.
         * @param x1 Start X position.
         * @param y1 Start Y position.
         * @param x2 End X position.
         * @param y2 End Y position.
         * @param color Line color.
         * @return Does not return a value.
         */
        virtual void drawLine(int x1, int y1, int x2, int y2, Arcade::Color color) = 0;
        /**
         * @brief Draws a filled tile.
         * @param x Tile X position.
         * @param y Tile Y position.
         * @param color Tile color.
         * @return Does not return a value.
         */
        virtual void drawTile(int x, int y, Arcade::Color color) = 0;
        /**
         * @brief Draws a single box tile.
         * @param x Tile X position.
         * @param y Tile Y position.
         * @param color Tile color.
         * @return Does not return a value.
         */
        virtual void drawbox(int x, int y, Arcade::Color color) = 0;
        /**
         * @brief Draws a sprite object list.
         * @param objects Sprite objects to render.
         * @return Does not return a value.
         */
        virtual void draw(const std::vector<Arcade::SpriteObject> &objects) = 0;
        /**
         * @brief Draws complete display data for one frame.
         * @param data Aggregated display data to render.
         * @return Does not return a value.
         */
        virtual void draw(const Arcade::DisplayData &data) = 0;
        /**
         * @brief Polls pending input commands.
         * @return Vector of detected input commands.
         */
        virtual std::vector<Arcade::InputCommand> pollInputs() = 0;
        /**
         * @brief Returns the current logical screen size.
         * @return Pair of width and height.
         */
        virtual std::pair<int,int> getScreenSize() = 0;

    protected:
    private:
};

#endif /* !IGRAPHIC_HPP_ */
