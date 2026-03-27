/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Player
*/

#include "core/Player.hpp"
#include <fstream>
#include <sstream>

/**
 * @brief Builds a player object with the provided display name.
 * @param name Player name used for profile identification.
 * @return Does not return a value.
 */
Player::Player(const std::string& name) : _name(name)
{
}

/**
 * @brief Retrieves the best score recorded for a game.
 * @param gameName Name of the game to query.
 * @return Stored high score value for this game.
 */
int Player::getHighScore(const std::string& gameName) const
{
    auto it = _highScores.find(gameName);
    return (it != _highScores.end()) ? it->second : 0;
}

/**
 * @brief Updates the high score for a given game.
 * @param gameName Name of the game to update.
 * @param score New high score value.
 * @return Does not return a value.
 */
void Player::setHighScore(const std::string& gameName, int score)
{
    if (_highScores.find(gameName) == _highScores.end() || _highScores[gameName] < score)
        _highScores[gameName] = score;
}

/**
 * @brief Saves the player profile and scores to disk.
 * @param filePath Destination file path.
 * @return Does not return a value.
 */
void Player::saveToFile(const std::string& filePath) const
{
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open())
        return;
    for (const auto& entry : _highScores) {
        const std::string name = _name;
        const int score = entry.second;
        file << name << ":" << score << "\n";
    }
    file.close();
}

/**
 * @brief Loads the player profile and scores from disk.
 * @param filePath Source file path.
 * @return Does not return a value.
 */
void Player::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        return;
    
    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;
        std::string gameName = line.substr(0, colonPos);
        int score = std::stoi(line.substr(colonPos + 1));
        _highScores[gameName] = score;
    }
    file.close();
}
