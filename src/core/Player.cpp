/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Player
*/

#include "core/Player.hpp"

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
    return 0;
}

/**
 * @brief Updates the high score for a given game.
 * @param gameName Name of the game to update.
 * @param score New high score value.
 * @return Does not return a value.
 */
void Player::setHighScore(const std::string& gameName, int score)
{

}

/**
 * @brief Saves the player profile and scores to disk.
 * @param filePath Destination file path.
 * @return Does not return a value.
 */
void Player::saveToFile(const std::string& filePath) const
{

}

/**
 * @brief Loads the player profile and scores from disk.
 * @param filePath Source file path.
 * @return Does not return a value.
 */
void Player::loadFromFile(const std::string& filePath)
{
    
}
