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
    MenuAction handleInput(const std::vector<Arcade::InputCommand>& commands);
    
    // Retourne la prochaine lib graphique pour le switch au runtime
    MenuAction getNextGraphic();

    // Fournit la taille logique de l'ecran pour produire un layout adapte
    void setScreenSize(int width, int height);

    // Exporte l'etat visuel du menu sous forme de donnees semantiques
    Arcade::DisplayData getDisplayables() const;

private:
    enum class LibraryType {
        UNKNOWN,
        GAME,
        GRAPHIC
    };

    static LibraryType detectLibraryType(const std::string &path);
    static int centerX(int screenW, const std::string &text);
    static void addBorderSprites(Arcade::DisplayData &data, int screenW, int screenH);
    static void addLibraryListTexts(Arcade::DisplayData &data, const std::vector<std::string> &libs,
        size_t selectedIndex, int startY, Arcade::Color selectedColor);
    static void addMissingLibrariesWarnings(Arcade::DisplayData &data, bool noGames, bool noGraphics,
        int currentY, int graphicCount);

    std::vector<std::string> _gameLibs;
    std::vector<std::string> _graphicLibs;
    std::string _playerName;
    int _screenWidth;
    int _screenHeight;
    
    size_t _selectedGameIndex;
    size_t _selectedGraphicIndex;
};

class MenuPage : public Menu {
    public:
    MenuPage() = default;
    ~MenuPage() = default;
};

class SettingsPage : public Menu {
    public:
    SettingsPage() = default;
    ~SettingsPage() = default;
};
#endif /* !MENU_HPP_ */
