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
        Player(const std::string& name);
        ~Player() = default;

        std::string getName() const { return _name; }
        int getHighScore(const std::string& gameName) const;
        void setHighScore(const std::string& gameName, int score);
        void saveToFile(const std::string& filePath) const;
        void loadFromFile(const std::string& filePath);

    private:
        std::string _name;
        std::map<std::string, int> _highScores;
};

#endif /* !PLAYER_HPP_ */
