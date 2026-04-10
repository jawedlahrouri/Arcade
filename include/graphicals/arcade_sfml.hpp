/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sfml
*/

#ifndef ARCADE_SFML_HPP_
#define ARCADE_SFML_HPP_

#include "IGraphic.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>
#include <memory>

class SfmlAdapter : public Arcade::IGraphic {
private:
    std::unique_ptr<sf::RenderWindow> _window;
    std::unordered_map<std::string, sf::Texture> _textures;
    std::unordered_map<std::string, sf::Font> _fonts;
    std::string _textInput;

    static const std::unordered_map<sf::Keyboard::Key, Arcade::InputCommand> KEY_MAP;
    std::unordered_map<Arcade::ElementType, std::string> _assetsMap;
    std::string _fontPath;  
    sf::Color getSfmlColor(Arcade::Color color);
    void drawTile(int x, int y, Arcade::Color color);
    void drawSprite(int x, int y, const std::string &spritePath);
    void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize);
    void draw(const std::vector<Arcade::SpriteObject> &objects);

public:
    SfmlAdapter() = default;
    ~SfmlAdapter() override;

    void init(const std::unordered_map<Arcade::ElementType, std::string> &assets, std::string font) override;
    void clear() override;
    void display() override;
    void draw(const Arcade::DisplayData &data) override;
    [[nodiscard]] std::vector<Arcade::InputCommand> getKeyboardEvents() override;
    [[nodiscard]] std::string getTextInput() override;
    [[nodiscard]] std::pair<int, int> getScreenSize() override;
};

#endif /* !ARCADE_SFML_HPP_ */
