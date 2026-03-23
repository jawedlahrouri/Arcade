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

namespace {
    const std::unordered_map<SDL_Keycode, Arcade::InputCommand> kSdlKeyMap = {
        {SDLK_UP, Arcade::InputCommand::UP},
        {SDLK_DOWN, Arcade::InputCommand::DOWN},
        {SDLK_LEFT, Arcade::InputCommand::LEFT},
        {SDLK_RIGHT, Arcade::InputCommand::RIGHT},
        {SDLK_RETURN, Arcade::InputCommand::ENTER},
        {SDLK_KP_ENTER, Arcade::InputCommand::ENTER},
        {SDLK_s, Arcade::InputCommand::SWITCH_GRAPHICS},
        {SDLK_ESCAPE, Arcade::InputCommand::BACK},
        {SDLK_q, Arcade::InputCommand::QUIT}
    };
}

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
public:
    void init() override;
    void clear() override;
    void display() override;
    void drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) override;
    void drawRect(int x, int y, int width, int height, ArcadeColor color) override;
    void drawCircle(int x, int y, int radius, ArcadeColor color) override;
    void drawSprite(int x, int y, const std::string &spritePath) override;
    void drawLine(int x1, int y1, int x2, int y2, ArcadeColor color) override;
    void drawTile(int x, int y, ArcadeColor color) override;
    std::vector<Arcade::InputCommand> pollInputs() override;
    std::pair<int,int> getScreenSize() override;
    ~Sdl2Adapter() override;

};

#endif /* !ARCADE_SDL2_HPP_ */
