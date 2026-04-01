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
    
    std::unique_ptr<DLLoader<Arcade::IGraphic>> _graphicLoader; 
    std::unique_ptr<Arcade::IGraphic> _graphic;
    
    std::unique_ptr<DLLoader<Arcade::IGames>> _gameLoader;
    std::unique_ptr<Arcade::IGames> _currentGame;
    std::string _currentGameLibPath;

    /**
     * @brief Checks whether enough time elapsed to process a new frame.
     * @param lastUpdate Timestamp of the previous processed frame.
     * @return true when the next frame can be rendered.
     */
    bool isFrameReady(std::chrono::steady_clock::time_point &lastUpdate);
    /**
     * @brief Applies a menu action to switch game or graphic backend.
     * @param action Action returned by the menu.
     * @param inMenu Indicates whether menu mode is active.
     * @return Does not return a value.
     */
    void applyMenuAction(const MenuAction &action, bool &inMenu);
    /**
     * @brief Switches to the next available graphic library.
     * @param arcadeMenu Menu instance providing the next graphic entry.
     * @return true when a switch happened.
     */
    bool handleSwitchGraphics(Menu &arcadeMenu);
    /**
     * @brief Reloads the currently active game library.
     * @param arcadeMenu Menu instance (unused but kept for parity).
     * @return true when the game was reloaded and the frame should be skipped.
     */
    bool handleReloadGame(Menu &arcadeMenu);
    /**
     * @brief Attempts to switch to a different game library.
     * @param gamePath Path to the game shared library.
     * @return true when a switch occurred.
     */
    bool switchGame(const std::string &gamePath);
    /**
     * @brief Handles one global command affecting core execution state.
     * @param command Command to handle.
     * @param inMenu Indicates whether menu mode is active.
     * @param skipFrame Output flag telling the loop to skip rendering.
     * @param arcadeMenu Menu instance used for runtime switches.
     * @return Does not return a value.
     */
    void handleCommand(const Arcade::InputCommand &command, bool &inMenu, bool &skipFrame, Menu &arcadeMenu);
    /**
     * @brief Processes all global commands for the current frame.
     * @param commands Commands collected from the input backend.
     * @param inMenu Indicates whether menu mode is active.
     * @param arcadeMenu Menu instance used by command handlers.
     * @return true when frame rendering should be skipped.
     */
    bool processGlobalCommands(const std::vector<Arcade::InputCommand> &commands, bool &inMenu, Menu &arcadeMenu);
    /**
     * @brief Updates menu state and renders one menu frame.
     * @param arcadeMenu Menu state manager.
     * @param commands Commands collected for this frame.
     * @param inMenu Indicates whether menu mode remains active.
     * @return Does not return a value.
     */
    void renderMenuFrame(Menu &arcadeMenu, const std::vector<Arcade::InputCommand> &commands, bool &inMenu);
    /**
     * @brief Updates and renders one game frame.
     * @param commands Commands collected for this frame.
     * @param inMenu Output flag set when returning to menu.
     * @return Does not return a value.
     */
    void renderGameFrame(const std::vector<Arcade::InputCommand> &commands, bool &inMenu);
    /**
     * @brief Runs one full frame pipeline from input to display.
     * @param arcadeMenu Menu state manager.
     * @param inMenu Indicates whether menu mode is active.
     * @return Does not return a value.
     */
    void runFrame(Menu &arcadeMenu, bool &inMenu);

    public:
    /**
     * @brief Builds the core and loads the initial graphic backend.
     * @param libPath Path to the initial graphic shared library.
     * @return Does not return a value.
     */
    Core(const std::string& libPath);
    /**
     * @brief Changes the active graphic library at runtime.
     * @param newLibPath Path to the new graphic shared library.
     * @return Does not return a value.
     */
    void changeGraphicLib(const std::string& newLibPath);
    /**
     * @brief Loads and initializes a game library.
     * @param gamePath Path to the game shared library.
     * @return Does not return a value.
     */
    void loadGame(const std::string& gamePath);
    /**
     * @brief Unloads the currently active game.
     * @return Does not return a value.
     */
    void unloadGame();
    /**
     * @brief Starts the main application loop.
     * @return Does not return a value.
     */
    void run();
    /**
     * @brief Returns the path of the active graphic library.
     * @return Current graphic library path.
     */
    std::string getCurrentGraphicLib() const { return _currentGraphicLibPath; }
    ~Core() = default;
};
#endif /* !CORE_HPP_ */
