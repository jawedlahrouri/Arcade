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
    /**
     * @brief Builds the menu and initializes its default state.
     * @return Does not return a value.
     */
    Menu();
    ~Menu() = default;

    /**
     * @brief Scans ./lib and classifies available game and graphic libraries.
     * @return Does not return a value.
     */
    void loadLibrariesList();
    
    /**
     * @brief Handles menu input commands and returns a resulting action.
     * @param commands List of commands for the current frame.
     * @return Menu action describing the requested operation.
     */
    MenuAction handleInput(const std::vector<Arcade::InputCommand>& commands);
    
    /**
     * @brief Selects the next graphic library for runtime switching.
     * @return Menu action containing the next graphic library path.
     */
    MenuAction getNextGraphic();

    /**
     * @brief Sets the logical screen size used for menu layout.
     * @param width Screen width in tiles.
     * @param height Screen height in tiles.
     * @return Does not return a value.
     */
    void setScreenSize(int width, int height);

    /**
     * @brief Builds visual menu data as semantic display objects.
     * @return Display data containing sprites and texts.
     */
    Arcade::DisplayData getDisplayables() const;

private:
    enum class LibraryType {
        UNKNOWN,
        GAME,
        GRAPHIC
    };

    /**
     * @brief Detects the type of a shared library from exported symbols.
     * @param path Path to the shared library.
     * @return Detected library type.
     */
    static LibraryType detectLibraryType(const std::string &path);
    /**
     * @brief Computes the centered X position for a text.
     * @param screenW Screen width in tiles.
     * @param text Text to center.
     * @return X coordinate for centered drawing.
     */
    static int centerX(int screenW, const std::string &text);
    /**
     * @brief Adds border wall sprites to the display data.
     * @param data Display data to enrich.
     * @param screenW Screen width in tiles.
     * @param screenH Screen height in tiles.
     * @return Does not return a value.
     */
    static void addBorderSprites(Arcade::DisplayData &data, int screenW, int screenH);
    /**
     * @brief Adds one text entry per library and highlights selection.
     * @param data Display data to enrich.
     * @param libs Library names to list.
     * @param selectedIndex Selected list index.
     * @param startY First Y row for the list.
     * @param selectedColor Color used for the selected entry.
     * @return Does not return a value.
     */
    static void addLibraryListTexts(Arcade::DisplayData &data, const std::vector<std::string> &libs,
        size_t selectedIndex, int startY, Arcade::Color selectedColor);
    /**
     * @brief Adds warnings when no game or graphic library is available.
     * @param data Display data to enrich.
     * @param noGames True when no game library was found.
     * @param noGraphics True when no graphic library was found.
     * @param currentY Current vertical cursor position.
     * @param graphicCount Number of listed graphic libraries.
     * @return Does not return a value.
     */
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
    /**
     * @brief Builds the menu page specialization.
     * @return Does not return a value.
     */
    MenuPage() = default;
    ~MenuPage() = default;
};

class SettingsPage : public Menu {
    public:
    /**
     * @brief Builds the settings page specialization.
     * @return Does not return a value.
     */
    SettingsPage() = default;
    ~SettingsPage() = default;
};
#endif /* !MENU_HPP_ */
