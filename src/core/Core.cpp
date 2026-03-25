/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Core
*/

#include "./core/core.hpp"
#include "./core/menu.hpp"
#include "./dll_cmds.hpp"
#include <iostream>
#include <chrono>
#include "./exception"
#include "exception.hpp"

Core::Core(const std::string& libPath) : _isRunning(true), _currentGraphicLibPath(libPath) {
    _graphicLoader = std::make_unique<DLLoader<IGraphic>>(libPath);
    IGraphic* rawGraphicPtr = nullptr;
    try {
        rawGraphicPtr = _graphicLoader->getInstance("createGraphical");
    } catch (Exception) {
        throw ErrorException("Error: " + libPath + " not a graphic library");
    }

    if (!rawGraphicPtr) {
        throw std::runtime_error("Failed to load graphic lib");
    }
    
    _graphic.reset(rawGraphicPtr);
}

void Core::changeGraphicLib(const std::string& newLibPath) {
    if (newLibPath.empty() || newLibPath == _currentGraphicLibPath) {
        return;
    }

    std::unique_ptr<DLLoader<IGraphic>> newLoader = std::make_unique<DLLoader<IGraphic>>(newLibPath);
    IGraphic* rawGraphicPtr = newLoader->getInstance("createGraphical"); 
    
    if (!rawGraphicPtr) {
        throw std::runtime_error("Failed to load new graphic lib");
    }
    std::unique_ptr<IGraphic> newGraphic(rawGraphicPtr);

    _graphic.reset();
    _graphicLoader.reset();
    _graphicLoader = std::move(newLoader);
    _graphic = std::move(newGraphic);
    _graphic->init();
    _currentGraphicLibPath = newLibPath;
}

void Core::loadGame(const std::string& gamePath) {
    if (gamePath.empty() || gamePath == _currentGameLibPath) {
        return;
    }

    unloadGame();

    _gameLoader = std::make_unique<DLLoader<IGames>>(gamePath);
    IGames* rawGamePtr = _gameLoader->getInstance("createGame");

    if (!rawGamePtr) {
        throw std::runtime_error("Failed to load game: " + gamePath);
    }

    _currentGame.reset(rawGamePtr);
    _currentGameLibPath = gamePath;
    _currentGame->init();
}

void Core::unloadGame() {
    _currentGame.reset();
    _gameLoader.reset();
    _currentGameLibPath = "";
}

bool Core::isFrameReady(std::chrono::steady_clock::time_point &lastUpdate)
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

    if (elapsed.count() < 16) {
        return false;
    }
    lastUpdate = now;
    return true;
}

void Core::applyMenuAction(const MenuAction &action, bool &inMenu)
{
    if (action.libPath.empty()) {
        return;
    }
    if (action.isGame) {
        loadGame(action.libPath);
        inMenu = false;
        return;
    }
    changeGraphicLib(action.libPath);
}

bool Core::handleSwitchGraphics(Menu &arcadeMenu)
{
    MenuAction action = arcadeMenu.getNextGraphic();

    if (action.libPath.empty()) {
        return false;
    }
    changeGraphicLib(action.libPath);
    _graphic->init();
    return true;
}

void Core::handleCommand(const Arcade::InputCommand &command, bool &inMenu, bool &skipFrame, Menu &arcadeMenu)
{
    if (command == Arcade::InputCommand::QUIT) {
        if (inMenu) {
            _isRunning = false;
            return;
        }
        inMenu = true;
        unloadGame();
        _graphic->clear();
        skipFrame = true;
        return;
    }
    if (command == Arcade::InputCommand::BACK && !inMenu) {
        unloadGame();
        inMenu = true;
        skipFrame = true;
        return;
    }
    if (command == Arcade::InputCommand::SWITCH_GRAPHICS) {
        skipFrame = handleSwitchGraphics(arcadeMenu);
    }
}

bool Core::processGlobalCommands(const std::vector<Arcade::InputCommand> &commands, bool &inMenu, Menu &arcadeMenu)
{
    bool skipFrame = false;

    for (const auto &command : commands) {
        handleCommand(command, inMenu, skipFrame, arcadeMenu);
        if (!_isRunning || skipFrame) {
            break;
        }
    }
    return skipFrame;
}

void Core::renderMenuFrame(Menu &arcadeMenu, const std::vector<Arcade::InputCommand> &commands, bool &inMenu)
{
    const std::pair<int, int> screenSize = _graphic->getScreenSize();
    arcadeMenu.setScreenSize(screenSize.first, screenSize.second);

    MenuAction action = arcadeMenu.handleInput(commands);
    applyMenuAction(action, inMenu);

    const Arcade::DisplayData menuDisplay = arcadeMenu.getDisplayables();
    _graphic->draw(menuDisplay);
}

void Core::renderGameFrame(const std::vector<Arcade::InputCommand> &commands, bool &inMenu)
{
    if (!_currentGame) {
        return;
    }
    _currentGame->handleInput(commands);
    _currentGame->update();

    const std::vector<Arcade::SpriteObject> objects = _currentGame->getDisplayables();
    _graphic->draw(objects);

    if (_currentGame->isGameOver()) {
        unloadGame();
        inMenu = true;
    }
}

void Core::runFrame(Menu &arcadeMenu, bool &inMenu)
{
    const std::vector<Arcade::InputCommand> commands = _graphic->pollInputs();
    const bool skipFrame = processGlobalCommands(commands, inMenu, arcadeMenu);

    if (!_isRunning || skipFrame) {
        return;
    }
    _graphic->clear();

    if (inMenu) {
        renderMenuFrame(arcadeMenu, commands, inMenu);
    } else {
        renderGameFrame(commands, inMenu);
    }
    _graphic->display();
}

void Core::run()
{
    if (!_graphic)
        return;

    _graphic->init();
    
    Menu arcadeMenu;
    arcadeMenu.loadLibrariesList();
    
    bool inMenu = true;
    auto lastUpdate = std::chrono::steady_clock::now();

    while (_isRunning) {
        if (!isFrameReady(lastUpdate)) {
            continue;
        }
        try {
            runFrame(arcadeMenu, inMenu);
        } catch (const std::exception &e) {
            std::cerr << "Failed to process frame: " << e.what() << std::endl;
            _isRunning = false;
        }
    }
}
