/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Igames
*/

#ifndef IGAMES_HPP_
#define IGAMES_HPP_
#include <unordered_map>
#include <vector>
#include <any>
#include "arcade_types.hpp"

namespace Arcade {
    class IGames {
    protected:
    private:

    public:
        virtual ~IGames() = default;

        [[nodiscard]] virtual std::any init() = 0;
        [[nodiscard]] virtual std::any update() = 0;
        virtual void handleKey(const std::vector<Arcade::InputCommand>& inputs) = 0;
        [[nodiscard]] virtual int getScore() = 0;
        [[nodiscard]] virtual bool isGameOver() = 0;
        [[nodiscard]] virtual Arcade::DisplayData getDisplayables() const = 0;
        [[nodiscard]] virtual std::unordered_map<Arcade::ElementType, std::string> getAssets() = 0;
    };
}

#endif /* !IGAMES_HPP_ */
