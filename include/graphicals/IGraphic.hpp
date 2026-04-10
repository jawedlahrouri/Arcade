/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** IGraphic
*/

#ifndef IGRAPHICAL_HPP_
#define IGRAPHICAL_HPP_
#include "arcade_types.hpp"
#include <string>
#include <utility>
#include <unordered_map>

// all the functions that the graphical library must implement
namespace Arcade {
class IGraphic {
    public:
        virtual ~IGraphic() = default;

        /**
         * @brief Initializes graphic backend resources.
         * @return Does not return a value.
         */
        virtual void init(const std::unordered_map<Arcade::ElementType, std::string> &assets, std::string font) = 0;
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
        [[nodiscard]] virtual std::string getTextInput() = 0;
        /**
         * @brief Polls pending input commands.
         * @return Vector of detected input commands.
         */
        [[nodiscard]] virtual std::vector<Arcade::InputCommand> getKeyboardEvents() = 0;
        /**
         * @brief Returns the current logical screen size.
         * @return Pair of width and height.
         */
        [[nodiscard]] virtual std::pair<int,int> getScreenSize() = 0;

    protected:
    private:
};
}

#endif /* !IGRAPHICAL_HPP_ */
