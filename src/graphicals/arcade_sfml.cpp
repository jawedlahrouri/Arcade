/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sfml
*/

#include "graphicals/arcade_sfml.hpp"
#include <iostream>
#include <map>

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
    _window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1000, 800), "Arcade - SFML");
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
    sfText.setPosition(x * 30.f, y * 30.f);
    sfText.setFillColor(getSfmlColor(color));
    _window->draw(sfText);
}

void SfmlAdapter::draw(const std::vector<Arcade::SpriteObject> &objects)
{
    float tSize = 40.f;
    static const std::map<Arcade::ElementType, std::string> texturePathMap = {
        {Arcade::ElementType::PLAYER_UP, "asset/textures/snake-head1.png"},
        {Arcade::ElementType::PLAYER_DOWN, "asset/textures/snake-head1.png"},
        {Arcade::ElementType::PLAYER_LEFT, "asset/textures/snake-head1.png"},
        {Arcade::ElementType::PLAYER_RIGHT, "asset/textures/snake-head1.png"},
        {Arcade::ElementType::PLAYER_TURN, "asset/textures/snake-turn1.png"},
        {Arcade::ElementType::PLAYER_TAIL, "asset/textures/snake-tail1.png"},
        {Arcade::ElementType::PLAYER_BODY, "asset/textures/snake-body1.png"},
        {Arcade::ElementType::FOOD, "asset/textures/snake-apple.png"},
        {Arcade::ElementType::WALL, "asset/textures/wall.png"},
    };

    if (!_window)
        return;
    for (size_t i = 0; i < objects.size(); i++) {
        const auto &obj = objects[i];
        const auto texIt = texturePathMap.find(obj.type);

        if (texIt == texturePathMap.end())
            continue;
        if (_textures.find(texIt->second) == _textures.end()) {
            sf::Texture tex;
            if (!tex.loadFromFile(texIt->second))
                continue;
            _textures[texIt->second] = tex;
        }
        sf::Sprite sprite(_textures[texIt->second]);
        double angle = 0.0;

        if (obj.type == Arcade::ElementType::PLAYER_UP)
            angle = 90.0;
        else if (obj.type == Arcade::ElementType::PLAYER_DOWN)
            angle = 270.0;
        else if (obj.type == Arcade::ElementType::PLAYER_LEFT)
            angle = 0.0;
        else if (obj.type == Arcade::ElementType::PLAYER_RIGHT)
            angle = 180.0;
        else if (obj.type == Arcade::ElementType::PLAYER_BODY) {
            if (i > 0) {
                const auto &prev = objects[i - 1];
                angle = (prev.x != obj.x) ? 0.0 : 90.0;
            }
        } else if (obj.type == Arcade::ElementType::PLAYER_TAIL) {
            if (i > 0) {
                const auto &prev = objects[i - 1];
                if (prev.x != obj.x)
                    angle = (prev.x > obj.x) ? 180.0 : 0.0;
                else
                    angle = (prev.y > obj.y) ? 270.0 : 90.0;
            }
        } else if (obj.type == Arcade::ElementType::PLAYER_TURN && i > 0 && i < objects.size() - 1) {
            const auto &prev = objects[i - 1];
            const auto &next = objects[i + 1];
            bool hasNeighborRight = (prev.x > obj.x || next.x > obj.x);
            bool hasNeighborLeft  = (prev.x < obj.x || next.x < obj.x);
            bool hasNeighborDown  = (prev.y > obj.y || next.y > obj.y);
            bool hasNeighborUp    = (prev.y < obj.y || next.y < obj.y);

            if (hasNeighborRight && hasNeighborDown)
                angle = 270.0;
            else if (hasNeighborLeft && hasNeighborDown)
                angle = 0.0;
            else if (hasNeighborLeft && hasNeighborUp)
                angle = 90.0;
            else if (hasNeighborRight && hasNeighborUp)
                angle = 180.0;
        }
        float scaleX = tSize / sprite.getLocalBounds().width;
        float scaleY = tSize / sprite.getLocalBounds().height;
        sprite.setScale(scaleX, scaleY);
        sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
        sprite.setRotation(angle);
        sprite.setPosition(obj.x * tSize + tSize / 2.f, obj.y * tSize + tSize / 2.f);
        _window->draw(sprite);
    }
}

void SfmlAdapter::draw(const Arcade::DisplayData &data)
{
    static const std::string defaultFont = "asset/font/coolvetica/Coolvetica Rg.otf";

    draw(data.sprites);
    for (const auto &textObj : data.texts) {
        drawText(textObj.x, textObj.y, textObj.content, textObj.color, defaultFont, 24);
    }
}

std::vector<Arcade::InputCommand> SfmlAdapter::getKeyboardEvents()
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

extern "C" { 
    int getType() {
        return 1;
    }
    
    Arcade::IGraphic* entryPoint() {
        return new SfmlAdapter();
    }
}

