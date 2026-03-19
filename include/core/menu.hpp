/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_
#include "graphicals/IGraphic.hpp"
#include <string>
#include <vector>

struct MenuAction {
    std::string libPath;
    bool isGame;
};

class Menu {
    public:
    Menu();
    ~Menu() = default;

    // Scan le dossier ./lib/ pour trouver les jeux et libs graphiques
    void loadLibrariesList();
    
    // Gère les actions
    MenuAction handleInput(const std::string& event);
    
    // Dessine le menu en utilisant la librairie graphique courante
    void draw(IGraphic& graphicLib);

private:
    std::vector<std::string> _gameLibs;
    std::vector<std::string> _graphicLibs;
    std::string _playerName;
    
    size_t _selectedGameIndex;
    size_t _selectedGraphicIndex;
};

#endif /* !MENU_HPP_ */
