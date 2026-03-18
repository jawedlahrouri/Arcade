/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** core
*/

#ifndef CORE_HPP_
#define CORE_HPP_
#include "graphicals/IGraphic.hpp"
#include <string>
#include <memory>
#include "./dll_cmds.hpp"
class Core {
    private:
    bool _isRunning;
    
    std::unique_ptr<DLLoader<IGraphic>> _graphicLoader; 
    std::unique_ptr<IGraphic> _graphic;

    public:
    Core(const std::string& libPath);
    void run();
    ~Core() = default;
};
#endif /* !CORE_HPP_ */
