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
         * @brief Draws a sprite object list.
         * @param objects Sprite objects to render.
         * @return Does not return a value.
         */
        virtual void draw(const Arcade::DisplayData &data) = 0;
        /**
         * @brief Polls pending input commands.
         * @return Vector of detected input commands.
         */
        virtual std::vector<Arcade::InputCommand> pollInputs() = 0;
        /**
         * @brief Gets the last text input from keyboard.
         * @return String containing typed characters, empty if none.
         */
        virtual std::string getTextInput() = 0;
        /**
         * @brief Returns the current logical screen size.
         * @return Pair of width and height.
         */
        virtual std::pair<int,int> getScreenSize() = 0;

    protected:
    private:
};

#endif /* !IGRAPHIC_HPP_ */
