/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sdl2
*/

#ifndef ARCADE_SDL2_HPP_
#define ARCADE_SDL2_HPP_

#include "./graphicals/IGraphic.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

class Sdl2AssetManager {
private:
    SDL_Renderer* _renderer;
    std::map<std::string, SDL_Texture*> _textures;

public:
    explicit Sdl2AssetManager(SDL_Renderer* renderer);
    ~Sdl2AssetManager();

    SDL_Texture* getTexture(const std::string& path);
};

class Sdl2Adapter : public IGraphic {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    const int tileSize = 32;
    std::map<std::string, TTF_Font*> fontsCache;
    std::unique_ptr<Sdl2AssetManager> assetManager;

    static const std::unordered_map<SDL_Keycode, Arcade::InputCommand> KEY_MAP;

public:
    void init() override;
    void clear() override;
    void display() override;
    void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) override;
    void drawRect(int x, int y, int width, int height, Arcade::Color color) override;
    void drawCircle(int x, int y, int radius, Arcade::Color color) override;
    void drawSprite(int x, int y, const std::string &spritePath) override;
    void drawLine(int x1, int y1, int x2, int y2, Arcade::Color color) override;
    void drawTile(int x, int y, Arcade::Color color) override;
    void draw(const std::vector<Arcade::GameObject> &objects) override;
    std::vector<Arcade::InputCommand> pollInputs() override;
    std::pair<int,int> getScreenSize() override;
    ~Sdl2Adapter() override;

};

#endif /* !ARCADE_SDL2_HPP_ */
