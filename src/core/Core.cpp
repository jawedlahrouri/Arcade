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
    _graphicLoader = new DLLoader<IGraphic>(libPath);
    _graphic = _graphicLoader->getInstance("createGraphical"); 
}

Core::~Core()
{
    delete _graphic;
    delete _graphicLoader;
}

void Core::run()
{
    if (!_graphic)
        return;
    _graphic->init();
    while (_isRunning) {
        try {
            _graphic->clear();
            _graphic->handleEvents();
            _graphic->display();
        } catch (const std::exception &e) {
            std::cerr << "Game requested exit: " << e.what() << std::endl;
            _isRunning = false;
        }
    }
}
