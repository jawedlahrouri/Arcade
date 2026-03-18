/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#include "core/menu.hpp"
#include "exception.hpp"
#include "graphicals/IGraphic.hpp"
#include <cstdio>
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
        {1, "arcade_sdl2.so"},
        {1, "arcade_sfml.so"}
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

void Menu::handleInput(const std::string& key) {
    // Exemple de logique de navigation
    if (key == "DOWN") {
        _selectedGameIndex = (_selectedGameIndex + 1) % _gameLibs.size();
    } else if (key == "UP") {
        _selectedGameIndex = (_selectedGameIndex > 0) ? _selectedGameIndex - 1 : _gameLibs.size() - 1;
    }
}

void Menu::draw(IGraphic& graphicLib) {
    auto screen = graphicLib.getScreenSize();
    int screenW = screen.first;
    int screenH = screen.second;

    int gameCount = static_cast<int>(_gameLibs.size());
    int graphCount = static_cast<int>(_graphicLibs.size());
    int totalLines = 4 + gameCount + graphCount;
    int startY = (screenH - totalLines) / 2;
    if (startY < 0) startY = 0;

    int y = startY;

    // Titre centré horizontalement
    std::string titleStr = "--- ARCADE MENU ---";
    int titleX = (screenW - (int)titleStr.length()) / 2;
    if (titleX < 0) titleX = 0;
    graphicLib.drawText(titleX, y, titleStr, ArcadeColor::YELLOW);
    y += 2;

    // Liste des jeux
    graphicLib.drawText(2, y, "Games available:", ArcadeColor::WHITE);
    y += 2;
    for (int i = 0; i < gameCount; ++i) {
        ArcadeColor color = (i == static_cast<int>(_selectedGameIndex)) ? ArcadeColor::RED : ArcadeColor::WHITE;
        graphicLib.drawText(4, y + i, _gameLibs[i], color);
    }
    y += gameCount + 1;

    //  Liste des libs graphiques
    graphicLib.drawText(2, y, "Graphics available:", ArcadeColor::WHITE);
    y += 2;
    for (int i = 0; i < graphCount; ++i) {
        ArcadeColor color = (i == static_cast<int>(_selectedGraphicIndex)) ? ArcadeColor::BLUE : ArcadeColor::WHITE;
        graphicLib.drawText(4, y + i, _graphicLibs[i], color);
    }
    y += graphCount + 1;

    //  Champ du nom du joueur
    // peut être mis ailleurs ou dans un sous menu TBD
    graphicLib.drawText(2, y, "Player Name: " + _playerName + "_", ArcadeColor::CYAN);
}
