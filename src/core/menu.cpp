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

namespace {
    enum class LibraryType {
        UNKNOWN,
        GAME,
        GRAPHIC
    };

    LibraryType detectLibraryType(const std::string &path)
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
}

Menu::Menu() : _playerName(""), _selectedGameIndex(0), _selectedGraphicIndex(0) {
    loadLibrariesList();
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
        const LibraryType type = detectLibraryType(filePath.string());
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

    // Box
    int boxWidth = screenW;
    int boxHeight = screenH;
    if (boxWidth <= 0 || boxHeight <= 0)
        return;
    graphicLib.drawRect(0, 0, boxWidth, boxHeight, Arcade::Color::WHITE);

    // Title
    std::string titleStr = "--- ARCADE MENU ---";
    int logicalTitleWidth = graphicLib.measureTextWidth(titleStr, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    int titleX = (screenW - logicalTitleWidth) / 2;
    if (titleX < 0)
        titleX = 0;
    graphicLib.drawText(titleX, 2, titleStr, Arcade::Color::YELLOW, "asset/font/coolvetica/Coolvetica Rg.otf", 24);


    // Draw Lib Games
    std::string titleGame = "--- GAMES AVAILABLE ---";
    int logicalTitleGameWidth = graphicLib.measureTextWidth(titleGame, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    int titleGameX = (screenW - logicalTitleGameWidth) / 2;
    if (titleGameX < 0)        
        titleGameX = 0;
    graphicLib.drawText(titleGameX, 6, titleGame, Arcade::Color::RED, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    for (size_t i = 0; i < _gameLibs.size(); ++i) {
        Arcade::Color color = (i == _selectedGameIndex) ? Arcade::Color::GREEN : Arcade::Color::WHITE;
        graphicLib.drawText(4, 8 + static_cast<int>(i), _gameLibs[i], color, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    }

    // Draw Lib Graphics
    std::string titleGraphics = "--- GRAPHICS AVAILABLE ---";
    int logicalTitleGraphicsWidth = graphicLib.measureTextWidth(titleGraphics, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    int titleGraphicsX = (screenW - logicalTitleGraphicsWidth) / 2;
    if (titleGraphicsX < 0)        
        titleGraphicsX = 0;
    graphicLib.drawText(titleGraphicsX, 12 + static_cast<int>(_gameLibs.size()), titleGraphics, Arcade::Color::BLUE, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    for (size_t i = 0; i < _graphicLibs.size(); ++i) {
        Arcade::Color color = (i == _selectedGraphicIndex) ? Arcade::Color::CYAN : Arcade::Color::WHITE;
        graphicLib.drawText(4, 14 + static_cast<int>(_gameLibs.size()) + static_cast<int>(i), _graphicLibs[i], color, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    }
}
