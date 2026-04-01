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

namespace {
    constexpr int GRAPHIC_LIBRARY_TYPE = 1;
    constexpr int GAME_LIBRARY_TYPE = 2;
}

/**
 * @brief Builds the core, loads the initial graphic library, and creates its instance.
 * @param libPath Path to the first graphic shared library.
 * @return Does not return a value.
 */
Core::Core(const std::string& libPath) : _isRunning(true), _currentGraphicLibPath(libPath) {
    _graphicLoader = std::make_unique<DLLoader<Arcade::IGraphic>>(libPath);
    Arcade::IGraphic* rawGraphicPtr = nullptr;
    try {
        if (_graphicLoader->getType() != GRAPHIC_LIBRARY_TYPE) {
            throw ErrorException("Error: " + libPath + " not a graphic library");
        }
        rawGraphicPtr = _graphicLoader->getInstance("entryPoint");
    } catch (const Exception &) {
        throw ErrorException("Error: " + libPath + " not a graphic library");
    }

    if (!rawGraphicPtr) {
        throw CrashException("Failed to load graphic lib");
    }
    
    _graphic.reset(rawGraphicPtr);
}

/**
 * @brief Replaces the active graphic backend with another shared library.
 * @param newLibPath Path to the new graphic shared library.
 * @return Does not return a value.
 */
void Core::changeGraphicLib(const std::string& newLibPath) {
    if (newLibPath.empty() || newLibPath == _currentGraphicLibPath) {
        return;
    }

    try {
        std::unique_ptr<DLLoader<Arcade::IGraphic>> newLoader = std::make_unique<DLLoader<Arcade::IGraphic>>(newLibPath);
        Arcade::IGraphic* rawGraphicPtr = nullptr;

        if (newLoader->getType() != GRAPHIC_LIBRARY_TYPE) {
            throw ErrorException("Error: " + newLibPath + " not a graphic library");
        }
        rawGraphicPtr = newLoader->getInstance("entryPoint");

        if (!rawGraphicPtr) {
            throw CrashException("Failed to load new graphic lib");
        }
        std::unique_ptr<Arcade::IGraphic> newGraphic(rawGraphicPtr);

        _graphic.reset();
        _graphicLoader.reset();
        _graphicLoader = std::move(newLoader);
        _graphic = std::move(newGraphic);
        _graphic->init();
        _currentGraphicLibPath = newLibPath;
    } catch (const Exception &) {
        throw;
    } catch (const std::exception &e) {
        throw CrashException("Failed to switch graphic library: " + std::string(e.what()));
    }
}

/**
 * @brief Loads a game library, instantiates it, and initializes the game.
 * @param gamePath Path to the game shared library.
 * @return Does not return a value.
 */
void Core::loadGame(const std::string& gamePath) {
    if (gamePath.empty() || gamePath == _currentGameLibPath) {
        return;
    }

    try {
        unloadGame();

        _gameLoader = std::make_unique<DLLoader<Arcade::IGames>>(gamePath);
        Arcade::IGames* rawGamePtr = nullptr;

        if (_gameLoader->getType() != GAME_LIBRARY_TYPE) {
            throw ErrorException("Error: " + gamePath + " not a game library");
        }
        rawGamePtr = _gameLoader->getInstance("entryPoint");

        if (!rawGamePtr) {
            throw CrashException("Failed to load game: " + gamePath);
        }

        _currentGame.reset(rawGamePtr);
        _currentGameLibPath = gamePath;
        _currentGame->init();
    } catch (const Exception &) {
        throw;
    } catch (const std::exception &e) {
        throw CrashException("Failed to load game library: " + gamePath + " (" + e.what() + ")");
    }
}

/**
 * @brief Unloads the currently running game and resets game-related resources.
 * @return Does not return a value.
 */
void Core::unloadGame() {
    _currentGame.reset();
    _gameLoader.reset();
    _currentGameLibPath = "";
}

/**
 * @brief Enforces a minimum frame interval before processing a new frame.
 * @param lastUpdate Timestamp of the previous accepted frame.
 * @return true when the next frame can be processed.
 */
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

/**
 * @brief Applies the menu action by loading a game or switching graphics.
 * @param action Menu action to apply.
 * @param inMenu Indicates whether the program is currently in menu mode.
 * @return Does not return a value.
 */
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

/**
 * @brief Selects and loads the next available graphic library.
 * @param arcadeMenu Menu object used to pick the next graphic library.
 * @return true when a switch occurred and the frame should be skipped.
 */
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

bool Core::switchGame(const std::string &gamePath)
{
    if (gamePath.empty() || gamePath == _currentGameLibPath) {
        return false;
    }
    loadGame(gamePath);
    return true;
}

bool Core::handleReloadGame(Menu &arcadeMenu)
{
    (void)arcadeMenu;
    if (_currentGameLibPath.empty()) {
        return false;
    }
    loadGame(_currentGameLibPath);
    return true;
}



/**
 * @brief Handles one global command affecting app state, menu, or backend.
 * @param command Input command to process.
 * @param inMenu Indicates whether the menu is currently active.
 * @param skipFrame Output flag to skip rendering for this frame.
 * @param arcadeMenu Menu object used for graphic cycling.
 * @return Does not return a value.
 */
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
    if (command == Arcade::InputCommand::SWITCH_GAME) {
        MenuAction action = arcadeMenu.getNextGame();
        if (!action.libPath.empty()) {
            loadGame(action.libPath);
            inMenu = false;
            skipFrame = true;
        }
    }
    if (command == Arcade::InputCommand::RELOAD_GAME) {
        skipFrame = handleReloadGame(arcadeMenu);
    }
}


/**
 * @brief Processes all global commands collected for the current frame.
 * @param commands List of commands to process.
 * @param inMenu Indicates whether menu mode is active.
 * @param arcadeMenu Menu object used by global actions.
 * @return true when frame rendering should be skipped.
 */
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

/**
 * @brief Renders one menu frame, including input handling and draw calls.
 * @param arcadeMenu Menu state and behavior handler.
 * @param commands Commands polled for this frame.
 * @param inMenu Indicates whether menu mode remains active.
 * @return Does not return a value.
 */
void Core::renderMenuFrame(Menu &arcadeMenu, const std::vector<Arcade::InputCommand> &commands, bool &inMenu)
{
    const std::pair<int, int> screenSize = _graphic->getScreenSize();
    const std::string textInput = _graphic->getTextInput();

    arcadeMenu.setScreenSize(screenSize.first, screenSize.second);

    MenuAction action = arcadeMenu.handleInput(commands, textInput);
    applyMenuAction(action, inMenu);

    const auto menuDisplay = arcadeMenu.getDisplayables();
    _graphic->draw(menuDisplay);
}

/**
 * @brief Updates and renders one game frame using current inputs.
 * @param commands Commands polled for this frame.
 * @param inMenu Output flag set when the game ends.
 * @return Does not return a value.
 */
void Core::renderGameFrame(const std::vector<Arcade::InputCommand> &commands, bool &inMenu)
{
    if (!_currentGame) {
        return;
    }
    _currentGame->handleKey(commands);
    _currentGame->update();

    const auto frameData = _currentGame->getDisplayables();
    _graphic->draw(frameData);

    if (_currentGame->isGameOver()) {
        unloadGame();
        inMenu = true;
    }
}

/**
 * @brief Executes one full frame pipeline: input, update, render, display.
 * @param arcadeMenu Menu state and actions handler.
 * @param inMenu Indicates whether menu mode is active.
 * @return Does not return a value.
 */
void Core::runFrame(Menu &arcadeMenu, bool &inMenu)
{
    try {
        const std::vector<Arcade::InputCommand> commands = _graphic->getKeyboardEvents();
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
    } catch (const Exception &) {
        throw;
    } catch (const std::exception &e) {
        throw CrashException("Frame execution failed: " + std::string(e.what()));
    }
}

/**
 * @brief Starts and runs the main loop until the application exits.
 * @return Does not return a value.
 */
void Core::run()
{
    if (!_graphic)
        return;

    Menu arcadeMenu;

    try {
        _graphic->init();
        arcadeMenu.loadLibrariesList();
    } catch (const Exception &) {
        throw;
    } catch (const std::exception &e) {
        throw CrashException("Core startup failed: " + std::string(e.what()));
    }
    
    bool inMenu = true;
    auto lastUpdate = std::chrono::steady_clock::now();

    while (_isRunning) {
        if (!isFrameReady(lastUpdate)) {
            continue;
        }
        try {
            runFrame(arcadeMenu, inMenu);
        } catch (const WarningException &e) {
            std::cerr << "Warning: " << e.what() << std::endl;
        } catch (const Exception &e) {
            std::cerr << "Failed to process frame: " << e.what() << std::endl;
            _isRunning = false;
        } catch (const std::exception &e) {
            std::cerr << "Unexpected runtime error: " << e.what() << std::endl;
            _isRunning = false;
        }
    }
}
