/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#include "core/menu.hpp"
#include "graphicals/IGraphic.hpp"
#include <iostream>
#include <vector>

Menu::Menu() : _playerName(""), _selectedGameIndex(0), _selectedGraphicIndex(0) {
    loadLibrariesList();
}

// Pour l'instant, on met des valeurs en dur pour tester l'affichage.
void Menu::loadLibrariesList() {
    _gameLibs.clear();
    _graphicLibs.clear();

    std::vector<std::pair<int, std::string>> libs = {
        {0, "arcade_snake.so"},
        {0, "arcade_pacman.so"},
        {1, "arcade_ncurses.so"},
        {1, "arcade_sdl2.so"}
    };
    for (const auto& lib : libs) {
        if (lib.first == 0) {
            std::cout << "[Menu] " << lib.second << " -> jeu (0)" << std::endl;
            _gameLibs.push_back(lib.second);
        } else if (lib.first == 1) {
            std::cout << "[Menu] " << lib.second << " -> lib graphique (1)" << std::endl;
            _graphicLibs.push_back(lib.second);
        } else {
            std::cout << "[Menu] " << lib.second << " -> type inconnu (" << lib.first << ")" << std::endl;
        }
    }
}

MenuAction Menu::handleInput(const std::vector<Arcade::InputCommand>& commands) {
    MenuAction action = {"", false};

    if (_gameLibs.empty() || _graphicLibs.empty())
        return action;

    for (const auto command : commands) {
        if (command == Arcade::InputCommand::DOWN) {
            if (_selectedGameIndex + 1 < _gameLibs.size()) {
                _selectedGameIndex++;
            }
            continue;
        }

        if (command == Arcade::InputCommand::UP) {
            if (_selectedGameIndex > 0) {
                _selectedGameIndex--;
            }
            continue;
        }

        if (command == Arcade::InputCommand::ENTER) {
            action.libPath = "./lib/" + _gameLibs[_selectedGameIndex];
            action.isGame = true;
            return action;
        }

        if (command == Arcade::InputCommand::SWITCH_GRAPHICS) {
            if (_selectedGraphicIndex + 1 < _graphicLibs.size()) {
                _selectedGraphicIndex++;
            } else {
                _selectedGraphicIndex = 0;
            }
            action.libPath = "./lib/" + _graphicLibs[_selectedGraphicIndex];
            action.isGame = false;
            return action;
        }
    }
    return action;
}

MenuAction Menu::getNextGraphic() {
    MenuAction action = {"", false};

    if (_graphicLibs.empty()) {
        return action;
    }

    if (_selectedGraphicIndex + 1 < _graphicLibs.size()) {
        _selectedGraphicIndex++;
    } else {
        _selectedGraphicIndex = 0;
    }
    action.libPath = "./lib/" + _graphicLibs[_selectedGraphicIndex];
    action.isGame = false;
    return action;
}

// void Menu::draw(IGraphic& graphicLib) {
//     static const std::string MenuFont = "asset/font/coolvetica/Coolvetica Rg.otf";
//     auto screen = graphicLib.getScreenSize();
//     int screenW = screen.first;
//     int screenH = screen.second;

//     int gameCount = static_cast<int>(_gameLibs.size());
//     int graphCount = static_cast<int>(_graphicLibs.size());
//     int totalLines = 4 + gameCount + graphCount;
//     int startY = (screenH - totalLines) / 2;
//     if (startY < 0) startY = 0;

//     int y = startY;

//     // Titre centré horizontalement
//     std::string titleStr = "--- ARCADE MENU ---";
//     int titleX = (screenW - (int)titleStr.length()) / 2;
//     if (titleX < 0) titleX = 0;
//     graphicLib.drawText(titleX, y, titleStr, Arcade::Color::YELLOW, MenuFont, 24);
//     y += 2;

//     // Liste des jeux
//     graphicLib.drawText(2, y, "Games available:", Arcade::Color::WHITE, MenuFont, 24);
//     y += 2;
//     for (int i = 0; i < gameCount; ++i) {
//         Arcade::Color color = (i == static_cast<int>(_selectedGameIndex)) ? Arcade::Color::RED : Arcade::Color::WHITE;
//         graphicLib.drawText(4, y + i, _gameLibs[i], color, MenuFont, 24);
//     }
//     y += gameCount + 1;

//     //  Liste des libs graphiques
//     graphicLib.drawText(2, y, "Graphics available:", Arcade::Color::WHITE, MenuFont, 24);
//     y += 2;
//     for (int i = 0; i < graphCount; ++i) {
//         Arcade::Color color = (i == static_cast<int>(_selectedGraphicIndex)) ? Arcade::Color::BLUE : Arcade::Color::WHITE;
//         graphicLib.drawText(4, y + i, _graphicLibs[i], color, MenuFont, 24);
//     }
//     y += graphCount + 1;

//     //  Champ du nom du joueur
//     // peut être mis ailleurs ou dans un sous menu TBD
//     graphicLib.drawText(2, y, "Player Name: " + _playerName + "_", Arcade::Color::CYAN, MenuFont, 24);
// }

void Menu::draw(IGraphic& graphicLib) {
    auto screen = graphicLib.getScreenSize();
    int screenW = screen.first;
    int screenH = screen.second;
    // Debug info
    std::string debugInfo = "Debug: " + std::to_string(screenW) + "x" + std::to_string(screenH);
    graphicLib.drawText(2, 1, debugInfo, Arcade::Color::MAGENTA, "asset/font/coolvetica/Coolvetica Rg.otf", 12);

    // Title
    std::string titleStr = "--- ARCADE MENU ---";
    int logicalTitleWidth = graphicLib.measureTextWidth(titleStr, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    int titleX = (screenW - logicalTitleWidth) / 2;
    if (titleX < 0) {
        titleX = 0;
    }
    graphicLib.drawText(titleX, 2, titleStr, Arcade::Color::YELLOW, "asset/font/coolvetica/Coolvetica Rg.otf", 24);

    // Box
    int boxWidth = screenW;
    int boxHeight = screenH;
    if (boxWidth <= 0 || boxHeight <= 0) {
        return;
    }
    graphicLib.drawRect(0, 0, boxWidth, boxHeight, Arcade::Color::WHITE);
}
