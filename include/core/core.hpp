/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** core
*/

#ifndef CORE_HPP_
#define CORE_HPP_
#include "graphicals/Igraphic.hpp"
#include <string>
#include "./dll_cmds.hpp"
class Core {
    private:
    bool _isRunning;
    DLLoader<IGraphic>* _graphicLoader; 
    IGraphic* _graphic;

    public:
    Core(const std::string& libPath);
    void run();
    ~Core();
};
#endif /* !CORE_HPP_ */
