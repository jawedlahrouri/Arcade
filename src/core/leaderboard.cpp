/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** leaderboard
*/

#include "core/leaderboard.hpp"
#include <fstream>
#include <algorithm>

Leaderboard::Leaderboard()
    : _playerName(""), _screenWidth(0), _screenHeight(0),
    _state(LeaderboardState::DISPLAY_SCORES)
{
    loadScores();
}

void Leaderboard::setScreenSize(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
}

void Leaderboard::beginNameInput()
{
    _playerName.clear();
    _state = LeaderboardState::INPUT_PLAYER_NAME;
}

void Leaderboard::loadScores()
{
    _scores.clear();
    std::ifstream file("scores.json");

    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line)) {
        size_t nameKey = line.find("\"name\"");
        size_t scoreKey = line.find("\"score\"");

        if (nameKey != std::string::npos && scoreKey != std::string::npos) {
            size_t firstQuote = line.find('"', nameKey + 6);
            size_t secondQuote = (firstQuote == std::string::npos) ? std::string::npos : line.find('"', firstQuote + 1);
            size_t colonPos = line.find(':', scoreKey);

            if (firstQuote != std::string::npos && secondQuote != std::string::npos && colonPos != std::string::npos) {
                std::string name = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                size_t numberStart = line.find_first_of("-0123456789", colonPos + 1);

                if (!name.empty() && numberStart != std::string::npos) {
                    try {
                        int score = std::stoi(line.substr(numberStart));
                        _scores[name] = score;
                        continue;
                    } catch (...) {
                    }
                }
            }
        }

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;
        std::string name = line.substr(0, colonPos);
        try {
            int score = std::stoi(line.substr(colonPos + 1));
            _scores[name] = score;
        } catch (...) {
            continue;
        }
    }
}

void Leaderboard::saveScores() const
{
    std::ofstream file("scores.json");

    if (!file.is_open())
        return;

    file << "[\n";
    size_t index = 0;
    for (const auto &entry : _scores) {
        file << "  {\"name\":\"" << entry.first << "\",\"score\":" << entry.second << "}";
        if (index + 1 < _scores.size())
            file << ",";
        file << "\n";
        ++index;
    }
    file << "]\n";
}

Leaderboard::LeaderboardState Leaderboard::handleInput(
    const std::vector<Arcade::InputCommand>& commands)
{
    if (_state == LeaderboardState::INPUT_PLAYER_NAME) {
        for (const auto cmd : commands) {
            if (cmd == Arcade::InputCommand::ENTER) {
                if (!_playerName.empty()) {
                    if (_scores.find(_playerName) == _scores.end())
                        _scores[_playerName] = 0;
                    saveScores();
                    _playerName.clear();
                }
                _state = LeaderboardState::DISPLAY_SCORES;
                return _state;
            }
            if (cmd == Arcade::InputCommand::BACK && !_playerName.empty()) {
                _playerName.pop_back();
                continue;
            }
            if (cmd == Arcade::InputCommand::QUIT || cmd == Arcade::InputCommand::SWITCH_LEADERBOARD) {
                _state = LeaderboardState::DISPLAY_SCORES;
                return _state;
            }
        }
        return _state;
    }

    for (const auto cmd : commands) {
        if (cmd == Arcade::InputCommand::QUIT) {
            return LeaderboardState::DISPLAY_SCORES;
        }
    }
    return _state;
}

void Leaderboard::addTextToPlayerName(const std::string& text)
{
    if (_playerName.size() < 15)
        _playerName += text;
}

void Leaderboard::addNameInputPrompt(Arcade::DisplayData &data) const
{
    int promptY = _screenHeight / 2 - 2;

    data.texts.push_back({"Enter player name:", 4, promptY, Arcade::Color::CYAN});
    data.texts.push_back({"> " + _playerName + "_", 4, promptY + 2, Arcade::Color::WHITE});
    data.texts.push_back({"Press ENTER to confirm", 4, promptY + 4, Arcade::Color::MAGENTA});
}

void Leaderboard::addBorderSprites(Arcade::DisplayData &data) const
{
    if (_screenWidth <= 1 || _screenHeight <= 1)
        return;
    for (int x = 0; x < _screenWidth; ++x) {
        data.sprites.push_back({Arcade::ElementType::WALL, x, 0});
        data.sprites.push_back({Arcade::ElementType::WALL, x, _screenHeight - 1});
    }
    for (int y = 1; y < _screenHeight - 1; ++y) {
        data.sprites.push_back({Arcade::ElementType::WALL, 0, y});
        data.sprites.push_back({Arcade::ElementType::WALL, _screenWidth - 1, y});
    }
}

void Leaderboard::addScoreEntries(Arcade::DisplayData &data) const
{
    std::vector<std::pair<std::string, int>> sorted(_scores.begin(), _scores.end());
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    data.texts.push_back({"--- LEADERBOARD ---", 4, 2, Arcade::Color::YELLOW});
    int y = 4;
    for (size_t i = 0; i < sorted.size() && y < _screenHeight - 2; ++i) {
        std::string entry = std::to_string(i + 1) + ". " + sorted[i].first
            + ": " + std::to_string(sorted[i].second);
        data.texts.push_back({entry, 4, y, Arcade::Color::WHITE});
        ++y;
    }
}

Arcade::DisplayData Leaderboard::getDisplayables() const
{
    Arcade::DisplayData data;
    addBorderSprites(data);

    if (_state == LeaderboardState::INPUT_PLAYER_NAME) {
        addNameInputPrompt(data);
    } else {
        addScoreEntries(data);
        data.texts.push_back({"Press L to return to menu", 2, _screenHeight - 2,
            Arcade::Color::MAGENTA});
    }
    return data;
}
