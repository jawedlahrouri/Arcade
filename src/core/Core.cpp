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

Core::Core(const std::string& libPath) : _isRunning(true) {
    _graphicLoader = std::make_unique<DLLoader<IGraphic>>(libPath);
    IGraphic* rawGraphicPtr = _graphicLoader->getInstance("createGraphical"); 
    
    if (!rawGraphicPtr) {
        throw std::runtime_error("Failed to load graphic lib");
    }
    
    _graphic.reset(rawGraphicPtr);
}

void Core::run()
{
    if (!_graphic)
        return;

    _graphic->init();
    
    Menu arcadeMenu;
    arcadeMenu.loadLibrariesList();
    
    bool inMenu = true;

    while (_isRunning) {
        try {
            _graphic->clear();
            
            std::string currentEvent = _graphic->handleEvents();
            
            if (currentEvent == "QUIT") {
                _isRunning = false;
            }

            if (inMenu) {
                arcadeMenu.handleInput(currentEvent);
                arcadeMenu.draw(*_graphic);
            } else {
                // Logique du jeu (ex: currentGame->update(), etc)
            }

            _graphic->display();
        } catch (const std::exception &e) {
            std::cerr << "Game requested exit: " << e.what() << std::endl;
            _isRunning = false;
        }
    }
}
