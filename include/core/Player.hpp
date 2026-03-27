/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include <map>

class Player {
    public:
        /**
         * @brief Builds a player profile with a given name.
         * @param name Player display name.
         * @return Does not return a value.
         */
        Player(const std::string& name);
        ~Player() = default;

        /**
         * @brief Returns the player name.
         * @return Player name string.
         */
        std::string getName() const { return _name; }
        /**
         * @brief Returns the stored high score for a game.
         * @param gameName Name of the game.
         * @return High score value for this game.
         */
        int getHighScore(const std::string& gameName) const;
        /**
         * @brief Stores or updates the high score for a game.
         * @param gameName Name of the game.
         * @param score New score value.
         * @return Does not return a value.
         */
        void setHighScore(const std::string& gameName, int score);
        /**
         * @brief Saves player data to a file.
         * @param filePath Destination file path.
         * @return Does not return a value.
         */
        void saveToFile(const std::string& filePath) const;
        /**
         * @brief Loads player data from a file.
         * @param filePath Source file path.
         * @return Does not return a value.
         */
        void loadFromFile(const std::string& filePath);

    private:
        std::string _name;
        std::map<std::string, int> _highScores;
};

#endif /* !PLAYER_HPP_ */
