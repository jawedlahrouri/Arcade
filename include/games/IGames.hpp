/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igames
*/

#ifndef IGAMES_HPP_
#define IGAMES_HPP_

#include <vector>
#include "arcade_types.hpp"

class IGames {
    public:
        virtual ~IGames() = default;

        /**
         * @brief Initializes game state before entering the loop.
         * @return Does not return a value.
         */
        virtual void init() = 0;
        /**
         * @brief Updates the game simulation for one frame.
         * @return Does not return a value.
         */
        virtual void update() = 0;
        /**
         * @brief Processes input commands for the current frame.
         * @param inputs List of input commands.
         * @return Does not return a value.
         */
        virtual void handleInput(const std::vector<Arcade::InputCommand>& inputs) = 0;
        /**
         * @brief Indicates whether the game reached a terminal state.
         * @return true when the game is over.
         */
        virtual bool isGameOver() = 0;
        /**
         * @brief Exports renderable objects representing the current frame.
         * @return Vector of sprite objects to draw.
         */
        virtual Arcade::DisplayData getDisplayables() const = 0;

    protected:
    private:
};

#endif /* !IGAMES_HPP_ */
