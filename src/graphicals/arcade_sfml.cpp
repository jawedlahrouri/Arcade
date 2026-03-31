/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sfml
*/

#include "graphicals/arcade_sfml.hpp"
#include <iostream>

const std::unordered_map<sf::Keyboard::Key, Arcade::InputCommand> SfmlAdapter::KEY_MAP = {
    {sf::Keyboard::Up, Arcade::InputCommand::UP},
    {sf::Keyboard::Down, Arcade::InputCommand::DOWN},
    {sf::Keyboard::Left, Arcade::InputCommand::LEFT},
    {sf::Keyboard::Right, Arcade::InputCommand::RIGHT},
    {sf::Keyboard::Enter, Arcade::InputCommand::ENTER},
    {sf::Keyboard::S, Arcade::InputCommand::SWITCH_GRAPHICS},
    {sf::Keyboard::R, Arcade::InputCommand::RELOAD_GAME},
    {sf::Keyboard::N, Arcade::InputCommand::SWITCH_GAME},
    {sf::Keyboard::L, Arcade::InputCommand::SWITCH_LEADERBOARD},
    {sf::Keyboard::Escape, Arcade::InputCommand::BACK},
    {sf::Keyboard::Space, Arcade::InputCommand::FLAG},
    {sf::Keyboard::Q, Arcade::InputCommand::QUIT}
};

SfmlAdapter::~SfmlAdapter()
{
    if (_window && _window->isOpen()) {
        _window->close();
    }
}

void SfmlAdapter::init()
{
    _window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Arcade - SFML");
    _window->setFramerateLimit(60);
}

void SfmlAdapter::clear()
{
    if (_window) {
        _window->clear(sf::Color::Black);
    }
}

void SfmlAdapter::display()
{
    if (_window) {
        _window->display();
    }
}

sf::Color SfmlAdapter::getSfmlColor(Arcade::Color color)
{
    switch (color) {
        case Arcade::Color::BLACK: return sf::Color::Black;
        case Arcade::Color::RED: return sf::Color::Red;
        case Arcade::Color::GREEN: return sf::Color::Green;
        case Arcade::Color::YELLOW: return sf::Color::Yellow;
        case Arcade::Color::BLUE: return sf::Color::Blue;
        case Arcade::Color::MAGENTA: return sf::Color::Magenta;
        case Arcade::Color::CYAN: return sf::Color::Cyan;
        case Arcade::Color::WHITE: return sf::Color::White;
        default: return sf::Color::White;
    }
}

void SfmlAdapter::drawTile(int x, int y, Arcade::Color color)
{
    if (!_window) return;
    sf::RectangleShape rect(sf::Vector2f(20.f, 20.f));
    rect.setPosition(x * 20.f, y * 20.f);
    rect.setFillColor(getSfmlColor(color));
    _window->draw(rect);
}

void SfmlAdapter::drawSprite(int x, int y, const std::string &spritePath)
{
    if (!_window) return;
    
    if (_textures.find(spritePath) == _textures.end()) {
        sf::Texture texture;
        if (texture.loadFromFile(spritePath)) {
            _textures[spritePath] = texture;
        } else {
            return;
        }
    }

    sf::Sprite sprite(_textures[spritePath]);
    sprite.setPosition(x * 20.f, y * 20.f);
    _window->draw(sprite);
}

void SfmlAdapter::drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize)
{
    if (!_window) return;

    if (_fonts.find(fontPath) == _fonts.end()) {
        sf::Font font;
        if (font.loadFromFile(fontPath)) {
            _fonts[fontPath] = font;
        } else {
            return;
        }
    }

    sf::Text sfText(text, _fonts[fontPath], fontSize);
    sfText.setPosition(x * 20.f, y * 20.f);
    sfText.setFillColor(getSfmlColor(color));
    _window->draw(sfText);
}

void SfmlAdapter::draw(const Arcade::DisplayData &data)
{
    for (const auto &obj : data.sprites) {
        Arcade::Color color = Arcade::Color::WHITE;
        switch (obj.type) {
            case Arcade::ElementType::WALL: color = Arcade::Color::BLUE; break;
            case Arcade::ElementType::PLAYER: color = Arcade::Color::YELLOW; break;
            case Arcade::ElementType::FOOD: color = Arcade::Color::RED; break;
            default: color = Arcade::Color::WHITE; break;
        }
        drawTile(obj.x, obj.y, color);
    }
    for (const auto &textObj : data.texts) {
        drawText(textObj.x, textObj.y, textObj.text, textObj.color, "asset/font/coolvetica/Coolvetica Rg.otf", 24);
    }
}

std::vector<Arcade::InputCommand> SfmlAdapter::pollInputs()
{
    std::vector<Arcade::InputCommand> commands;
    if (!_window) return commands;

    sf::Event event;
    while (_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            commands.push_back(Arcade::InputCommand::QUIT);
        } else if (event.type == sf::Event::KeyPressed) {
            auto it = KEY_MAP.find(event.key.code);
            if (it != KEY_MAP.end()) {
                commands.push_back(it->second);
            }
        } else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                _textInput += static_cast<char>(event.text.unicode);
            }
        }
    }
    
    return commands;
}

std::string SfmlAdapter::getTextInput()
{
    std::string input = _textInput;
    _textInput.clear();
    return input;
}

std::pair<int, int> SfmlAdapter::getScreenSize()
{
    if (_window) {
        sf::Vector2u size = _window->getSize();
        return {size.x / 20, size.y / 20};
    }
    return {0, 0};
}

extern "C" int getType() {
    return 1;
}

extern "C" IGraphic* entryPoint() {
    return new SfmlAdapter();
}

