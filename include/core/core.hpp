/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** core
*/

#ifndef CORE_HPP_
#define CORE_HPP_
#include "graphicals/IGraphic.hpp"
#include "games/IGames.hpp"
#include "core/menu.hpp"
#include <string>
#include <memory>
#include <chrono>
#include "./dll_cmds.hpp"

class Core {
    private:
    bool _isRunning;
    std::string _currentGraphicLibPath;
    
    std::unique_ptr<DLLoader<IGraphic>> _graphicLoader; 
    std::unique_ptr<IGraphic> _graphic;
    
    std::unique_ptr<DLLoader<IGames>> _gameLoader;
    std::unique_ptr<IGames> _currentGame;
    std::string _currentGameLibPath;

    bool isFrameReady(std::chrono::steady_clock::time_point &lastUpdate);
    void applyMenuAction(const MenuAction &action, bool &inMenu);
    bool handleSwitchGraphics(Menu &arcadeMenu);
    void handleCommand(const Arcade::InputCommand &command, bool &inMenu, bool &skipFrame, Menu &arcadeMenu);
    bool processGlobalCommands(const std::vector<Arcade::InputCommand> &commands, bool &inMenu, Menu &arcadeMenu);
    void renderMenuFrame(Menu &arcadeMenu, const std::vector<Arcade::InputCommand> &commands, bool &inMenu);
    void renderGameFrame(const std::vector<Arcade::InputCommand> &commands, bool &inMenu);
    void runFrame(Menu &arcadeMenu, bool &inMenu);

    public:
    Core(const std::string& libPath);
    void changeGraphicLib(const std::string& newLibPath);
    void loadGame(const std::string& gamePath);
    void unloadGame();
    void run();
    std::string getCurrentGraphicLib() const { return _currentGraphicLibPath; }
    ~Core() = default;
};
#endif /* !CORE_HPP_ */
