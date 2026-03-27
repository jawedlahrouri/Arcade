/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** leaderboard
*/

#ifndef LEADERBOARD_HPP_
#define LEADERBOARD_HPP_

#include "arcade_types.hpp"
#include <vector>
#include <string>
#include <map>

class Leaderboard {
    public:
    enum class LeaderboardState {
        DISPLAY_SCORES,
        INPUT_PLAYER_NAME
    };

    /**
     * @brief Builds leaderboard and loads scores from file.
     * @return Does not return a value.
     */
    Leaderboard();
    ~Leaderboard() = default;

    /**
     * @brief Sets the logical screen size for leaderboard layout.
     * @param width Screen width in tiles.
     * @param height Screen height in tiles.
     * @return Does not return a value.
     */
    void setScreenSize(int width, int height);

    /**
     * @brief Starts player name input mode.
     * @return Does not return a value.
     */
    void beginNameInput();

    /**
     * @brief Handles leaderboard commands and updates page state.
     * @param commands Input command list for the current frame.
     * @return Current leaderboard state after handling.
     */
    LeaderboardState handleInput(const std::vector<Arcade::InputCommand>& commands);

    /**
     * @brief Gets the current player name being entered.
     * @return Current player name string.
     */
    std::string getPlayerName() const { return _playerName; }

    /**
     * @brief Indicates whether the leaderboard is in name input mode.
     * @return true when waiting for player name typing.
     */
    bool isInNameInputMode() const { return _state == LeaderboardState::INPUT_PLAYER_NAME; }

    /**
     * @brief Adds text to player name input.
     * @param text Text to append.
     * @return Does not return a value.
     */
    void addTextToPlayerName(const std::string& text);

    /**
     * @brief Clears the player name input.
     * @return Does not return a value.
     */
    void clearPlayerName() { _playerName.clear(); }

    /**
     * @brief Draws input prompt for player name.
     * @param data Display data to enrich.
     * @return Does not return a value.
     */
    void addNameInputPrompt(Arcade::DisplayData &data) const;

    /**
     * @brief Adds border wall sprites to display data.
     * @param data Display data to enrich.
     * @return Does not return a value.
     */
    void addBorderSprites(Arcade::DisplayData &data) const;

    /**
     * @brief Draws the score entries sorted by rank.
     * @param data Display data to enrich.
     * @return Does not return a value.
     */
    void addScoreEntries(Arcade::DisplayData &data) const;

    /**
     * @brief Builds all leaderboard displayables for the active state.
     * @return Display data containing sprites and texts.
     */
    Arcade::DisplayData getDisplayables() const;

    private:
    /**
     * @brief Loads score entries from scores file.
     * @return Does not return a value.
     */
    void loadScores();

    /**
     * @brief Saves score entries to scores file.
     * @return Does not return a value.
     */
    void saveScores() const;

    std::map<std::string, int> _scores;
    std::string _playerName;
    int _screenWidth;
    int _screenHeight;
    LeaderboardState _state;
};

#endif /* !LEADERBOARD_HPP_ */
