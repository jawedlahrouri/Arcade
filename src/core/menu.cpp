/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#include "core/menu.hpp"
#include "exception.hpp"
#include "graphicals/IGraphic.hpp"

Menu::Menu() : _playerName(""), _selectedGameIndex(0), _selectedGraphicIndex(0) {
    loadLibrariesList();
}

// Pour l'instant, on met des valeurs en dur pour tester l'affichage.
void Menu::loadLibrariesList() {
    _gameLibs = {"arcade_snake.so"};
    _graphicLibs = {"arcade_ncurses.so", "arcade_sdl2.so", "arcade_sfml.so"};
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
    // on clear
    graphicLib.clear();

    // Calcul  pour centrer le menu à l'écran
    auto screen = graphicLib.getScreenSize();
    int screenW = screen.first;
    int screenH = screen.second;

    int gameCount = static_cast<int>(_gameLibs.size());
    int graphCount = static_cast<int>(_graphicLibs.size());
    int totalLines = 4 + gameCount + graphCount;
    int startY = (screenH - totalLines) / 2;
    if (startY < 0) startY = 0;

    int y = startY;

    //  Titre
    graphicLib.drawText(10, y, "--- ARCADE MENU ---", ArcadeColor::YELLOW);
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

    //  Mise à jour de l'écran
    graphicLib.display();
}
