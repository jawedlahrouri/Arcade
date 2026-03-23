/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_snake
*/
#include "games/arcade_snake.hpp"
#include "graphicals/IGraphic.hpp"

void SnakeGame::draw(IGraphic& graphicLib) {
    auto screen = graphicLib.getScreenSize();
    int screenW = screen.first;
    int screenH = screen.second;

    int startY = screenH / 4;
    int y = startY;

    // Titre centré horizontalement
    std::string titleStr = "--- Snake ---";
    int titleX = (screenW - (int)titleStr.length()) / 2;
    if (titleX < 0) titleX = 0;
    graphicLib.drawText(titleX, y, titleStr, ArcadeColor::YELLOW);
    y += 2;
}

void SnakeGame::init(IGraphic &graphicLib) {
    _isGameOver = false;
    SnakeGame::draw(graphicLib);
}

void SnakeGame::update() {
    // TODO: Update game logic here
}

void SnakeGame::handleInput(const std::vector<Arcade::InputCommand>& inputs) {
    for (const auto input : inputs) {
        if (input == Arcade::InputCommand::BACK || input == Arcade::InputCommand::QUIT) {
            _isGameOver = true;
            return;
        }
    }
    // TODO: Handle other inputs (movement, etc)
}

extern "C" IGames* createGame() {
    return new SnakeGame();
}