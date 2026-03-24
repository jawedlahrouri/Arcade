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
    _currentGame->init(*_graphic);
}

void Core::unloadGame() {
    _currentGame.reset();
    _gameLoader.reset();
    _currentGameLibPath = "";
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
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

        if (elapsed.count() < 16) { // ~60 FPS
            continue;
        }

        lastUpdate = now;
        try {
            std::vector<Arcade::InputCommand> commands = _graphic->pollInputs();
            bool skipFrame = false;

            for (const auto command : commands) {
                if (command == Arcade::InputCommand::QUIT) {
                    if (inMenu) {
                        _isRunning = false;
                    } else {
                        inMenu = true;
                        unloadGame();
                        _graphic->clear();
                        skipFrame = true;
                    }
                }
                if (command == Arcade::InputCommand::BACK && !inMenu) {
                    unloadGame();
                    inMenu = true;
                    skipFrame = true;
                }
                if (command == Arcade::InputCommand::SWITCH_GRAPHICS) {
                    MenuAction action = arcadeMenu.getNextGraphic();
                    if (!action.libPath.empty()) {
                        try {
                            changeGraphicLib(action.libPath);
                            _graphic->init();
                            skipFrame = true;
                        } catch (const std::exception &e) {
                            std::cerr << "Failed to switch graphic lib: " << e.what() << std::endl;
                        }
                    }
                }
            }

            if (!_isRunning || skipFrame) {
                continue;
            }

            _graphic->clear();

            if (inMenu) {
                MenuAction action = arcadeMenu.handleInput(commands);

                if (!action.libPath.empty()) {
                    try {
                        if (action.isGame) {
                            loadGame(action.libPath);
                            inMenu = false;
                        } else {
                            changeGraphicLib(action.libPath);
                        }
                    } catch (const std::exception &e) {
                        std::cerr << "Failed to load library: " << e.what() << std::endl;
                    }
                }
                arcadeMenu.draw(*_graphic);
            } else {
                // Jeu en cours
                if (_currentGame) {
                    _currentGame->handleInput(commands);
                    _currentGame->update();
                    _currentGame->draw(*_graphic);
                    const std::vector<Arcade::GameObject> objects = _currentGame->getDisplayables();
                    _graphic->draw(objects);

                    if (_currentGame->isGameOver()) {
                        unloadGame();
                        inMenu = true;
                    }
                }
            }

            _graphic->display();
            
        } catch (const std::exception &e) {
            std::cerr << "Game requested exit: " << e.what() << std::endl;
            _isRunning = false;
        }
    }
}
