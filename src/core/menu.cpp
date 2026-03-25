/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** menu
*/

#include "core/menu.hpp"
#include "graphicals/IGraphic.hpp"
#include <vector>
#include <algorithm>
#include <filesystem>
#include <dlfcn.h>

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

int Menu::centerX(int screenW, const std::string &text)
{
    if (screenW <= 0)
        return 2;
    const int logicalTextWidth = static_cast<int>((text.size() + 1) / 2);
    const int centered = (screenW - logicalTextWidth) / 2;
    return std::max(0, centered);
}

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

void Menu::addLibraryListTexts(Arcade::DisplayData &data, const std::vector<std::string> &libs,
    size_t selectedIndex, int startY, Arcade::Color selectedColor)
{
    for (size_t i = 0; i < libs.size(); ++i) {
        const Arcade::Color color = (i == selectedIndex) ? selectedColor : Arcade::Color::WHITE;
        data.texts.push_back({libs[i], 4, startY + static_cast<int>(i), color});
    }
}

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

Menu::Menu() : _playerName(""), _screenWidth(0), _screenHeight(0), _selectedGameIndex(0), _selectedGraphicIndex(0) {
    loadLibrariesList();
}

void Menu::setScreenSize(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
}

// load dynamic libraries from ./lib/ and classify them as games or graphics based on their bool
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

Arcade::DisplayData Menu::getDisplayables() const
{
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
    return data;
}
