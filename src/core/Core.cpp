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
    auto lastUpdate = std::chrono::steady_clock::now();

    while (_isRunning) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);

        if (elapsed.count() < 16) { // ~60 FPS
            continue;

        lastUpdate = now;
        try {
            std::string currentEvent = _graphic->handleEvents();
            
            if (currentEvent == "QUIT") {
                _isRunning = false;
            }

            _graphic->clear();

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
