/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#include "core/menu.hpp"
#include <vector>
#include <algorithm>
#include <filesystem>
#include <dlfcn.h>

/**
 * @brief Detects whether a shared library exports game or graphic symbols.
 * @param path Absolute or relative path to the .so file.
 * @return LibraryType::GAME, GRAPHIC, or UNKNOWN.
 */
Menu::LibraryType Menu::detectLibraryType(const std::string &path)
{
    void *handle = ::dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);

    if (!handle)
        return LibraryType::UNKNOWN;
    ::dlerror();
    void *isGameSym = ::dlsym(handle, "isGame");
    const char *flagError = ::dlerror();

    if (!flagError && isGameSym) {
        bool isGame = *reinterpret_cast<bool *>(isGameSym);
        ::dlclose(handle);
        return isGame ? LibraryType::GAME : LibraryType::GRAPHIC;
    }
    ::dlerror();
    void *gameFactory = ::dlsym(handle, "createGame");
    const char *gameError = ::dlerror();

    ::dlerror();
    void *graphicFactory = ::dlsym(handle, "createGraphical");
    const char *graphicError = ::dlerror();

    ::dlclose(handle);

    if (!gameError && gameFactory)
        return LibraryType::GAME;
    if (!graphicError && graphicFactory)
        return LibraryType::GRAPHIC;
    return LibraryType::UNKNOWN;
}

/**
 * @brief Computes a centered X position for a text in tile coordinates.
 * @param screenW Screen width in tiles.
 * @param text Text to center.
 * @return Left X coordinate where the text should start.
 */
int Menu::centerX(int screenW, const std::string &text)
{
    if (screenW <= 0)
        return 2;
    const int logicalTextWidth = static_cast<int>((text.size() + 1) / 2);
    const int centered = (screenW - logicalTextWidth) / 2;
    return std::max(0, centered);
}

/**
 * @brief Appends border wall sprites around the screen bounds.
 * @param data Display buffer to extend.
 * @param screenW Screen width in tiles.
 * @param screenH Screen height in tiles.
 * @return Does not return a value.
 */
void Menu::addBorderSprites(Arcade::DisplayData &data, int screenW, int screenH)
{
    if (screenW <= 1 || screenH <= 1)
        return;
    for (int x = 0; x < screenW; ++x) {
        data.sprites.push_back({Arcade::ElementType::WALL, x, 0});
        data.sprites.push_back({Arcade::ElementType::WALL, x, screenH - 1});
    }
    for (int y = 1; y < screenH - 1; ++y) {
        data.sprites.push_back({Arcade::ElementType::WALL, 0, y});
        data.sprites.push_back({Arcade::ElementType::WALL, screenW - 1, y});
    }
}

/**
 * @brief Adds one text line per library and highlights the selected entry.
 * @param data Display buffer to extend.
 * @param libs Library names to display.
 * @param selectedIndex Index of the highlighted entry.
 * @param startY First Y row used for the list.
 * @param selectedColor Color used for the selected item.
 * @return Does not return a value.
 */
void Menu::addLibraryListTexts(Arcade::DisplayData &data, const std::vector<std::string> &libs,
    size_t selectedIndex, int startY, Arcade::Color selectedColor)
{
    for (size_t i = 0; i < libs.size(); ++i) {
        const Arcade::Color color = (i == selectedIndex) ? selectedColor : Arcade::Color::WHITE;
        data.texts.push_back({libs[i], 4, startY + static_cast<int>(i), color});
    }
}

/**
 * @brief Adds warning messages when game or graphic libraries are missing.
 * @param data Display buffer to extend.
 * @param noGames True when no game library is available.
 * @param noGraphics True when no graphic library is available.
 * @param currentY Current Y cursor used to place warnings.
 * @param graphicCount Number of displayed graphic libraries.
 * @return Does not return a value.
 */
void Menu::addMissingLibrariesWarnings(Arcade::DisplayData &data, bool noGames, bool noGraphics,
    int currentY, int graphicCount)
{
    if (noGames) {
        data.texts.push_back({"No game library found in ./lib", 2,
            currentY + graphicCount + 2, Arcade::Color::YELLOW});
    }
    if (noGraphics) {
        data.texts.push_back({"No graphic library found in ./lib", 2,
            currentY + graphicCount + 3, Arcade::Color::YELLOW});
    }
}

/**
 * @brief Builds the menu with default state and loads available libraries.
 * @return Does not return a value.
 */
Menu::Menu() : _playerName(""), _screenWidth(0), _screenHeight(0), _selectedGameIndex(0), _selectedGraphicIndex(0),
    _currentPage(Page::MAIN_MENU), _leaderboard(std::make_unique<Leaderboard>())
{
    loadLibrariesList();
}

/**
 * @brief Updates the menu viewport dimensions used for layout.
 * @param width Screen width in tiles.
 * @param height Screen height in tiles.
 * @return Does not return a value.
 */
void Menu::setScreenSize(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
    _leaderboard->setScreenSize(width, height);
}

/**
 * @brief Scans ./lib and classifies .so files as game or graphic libraries.
 * @return Does not return a value.
 */
void Menu::loadLibrariesList() {
    _gameLibs.clear();
    _graphicLibs.clear();

    const std::filesystem::path libDir("./lib");
    if (!std::filesystem::exists(libDir) || !std::filesystem::is_directory(libDir))
        return;
    for (const auto &entry : std::filesystem::directory_iterator(libDir)) {
        if (!entry.is_regular_file())
            continue;
        const std::filesystem::path filePath = entry.path();
        if (filePath.extension() != ".so")
            continue;
        const std::string fileName = filePath.filename().string();
        const LibraryType type = Menu::detectLibraryType(filePath.string());
        if (type == LibraryType::GAME)
            _gameLibs.push_back(fileName);
        else if (type == LibraryType::GRAPHIC)
            _graphicLibs.push_back(fileName);
    }
    std::sort(_gameLibs.begin(), _gameLibs.end());
    std::sort(_graphicLibs.begin(), _graphicLibs.end());
    if (_selectedGameIndex >= _gameLibs.size())
        _selectedGameIndex = 0;
    if (_selectedGraphicIndex >= _graphicLibs.size())
        _selectedGraphicIndex = 0;
}

/**
 * @brief Handles menu navigation commands and returns the selected action.
 * @param commands Input command list for the current frame.
 * @return MenuAction describing the requested game or graphic change.
 */
MenuAction Menu::handleInput(const std::vector<Arcade::InputCommand>& commands,
    const std::string &textInput) {
    MenuAction action = {"", false};

    if (_currentPage == Page::LEADERBOARD) {
        if (!textInput.empty() && _leaderboard->isInNameInputMode()) {
            _leaderboard->addTextToPlayerName(textInput);
        }
        _leaderboard->handleInput(commands);
        for (const auto command : commands) {
            if (command == Arcade::InputCommand::SWITCH_LEADERBOARD &&
                !_leaderboard->isInNameInputMode()) {
                _currentPage = Page::MAIN_MENU;
                return action;
            }
        }
        return action;
    }

    for (const auto command : commands) {
        if (command == Arcade::InputCommand::SWITCH_LEADERBOARD) {
            _currentPage = Page::LEADERBOARD;
            _leaderboard->beginNameInput();
            return action;
        }
    }

    if (_gameLibs.empty() || _graphicLibs.empty())
        return action;

    for (const auto command : commands) {
        if (command == Arcade::InputCommand::DOWN) {
            if (_selectedGameIndex + 1 < _gameLibs.size())
                _selectedGameIndex++;
            continue;
        }

        if (command == Arcade::InputCommand::UP) {
            if (_selectedGameIndex > 0)
                _selectedGameIndex--;
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

/**
 * @brief Selects the next graphic library in cyclic order.
 * @return MenuAction for the next graphic library, or empty action.
 */
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

/**
 * @brief Changes the active page (main menu or leaderboard).
 * @param page New page to display.
 * @return Does not return a value.
 */
void Menu::setCurrentPage(Page page)
{
    _currentPage = page;
}

/**
 * @brief Selects the next game library in cyclic order.
 * @return MenuAction for the next game library, or empty action.
 */
MenuAction Menu::getNextGame() {
    MenuAction action = {"", false};

    if (_gameLibs.empty()) {
        return action;
    }

    if (_selectedGameIndex + 1 < _gameLibs.size()) {
        _selectedGameIndex++;
    } else {
        _selectedGameIndex = 0;
    }
    action.libPath = "./lib/" + _gameLibs[_selectedGameIndex];
    action.isGame = true;
    return action;
}

/**
 * @brief Builds all menu sprites and texts for rendering.
 * @return Fully populated display data for the current menu state.
 */
Arcade::DisplayData Menu::getDisplayables() const
{
    if (_currentPage == Page::LEADERBOARD)
        return _leaderboard->getDisplayables();

    Arcade::DisplayData data;
    int currentY = 2;
    const int screenW = _screenWidth;
    const int screenH = _screenHeight;
    const std::string debugInfo = "Debug: " + std::to_string(screenW) + "x" + std::to_string(screenH);

    Menu::addBorderSprites(data, screenW, screenH);

    data.texts.push_back({debugInfo, 2, 1, Arcade::Color::MAGENTA});

    const std::string menuTitle = "--- ARCADE MENU ---";
    const std::string gamesTitle = "--- GAMES AVAILABLE ---";
    const std::string graphicsTitle = "--- GRAPHICS AVAILABLE ---";

    data.texts.push_back({menuTitle, Menu::centerX(screenW, menuTitle), currentY, Arcade::Color::YELLOW});
    currentY += 2;

    data.texts.push_back({gamesTitle, Menu::centerX(screenW, gamesTitle), currentY, Arcade::Color::RED});
    currentY += 2;
    Menu::addLibraryListTexts(data, _gameLibs, _selectedGameIndex, currentY, Arcade::Color::GREEN);
    currentY += static_cast<int>(_gameLibs.size()) + 1;

    data.texts.push_back({graphicsTitle, Menu::centerX(screenW, graphicsTitle), currentY, Arcade::Color::BLUE});
    currentY += 2;
    Menu::addLibraryListTexts(data, _graphicLibs, _selectedGraphicIndex, currentY, Arcade::Color::CYAN);

    Menu::addMissingLibrariesWarnings(data, _gameLibs.empty(), _graphicLibs.empty(),
        currentY, static_cast<int>(_graphicLibs.size()));
    
    data.texts.push_back({"Press L for leaderboard", 2, screenH - 2, Arcade::Color::MAGENTA});
    return data;
}
