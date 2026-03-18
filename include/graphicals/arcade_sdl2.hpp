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

class Sdl2Adapter : public IGraphic {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    const int tileSize = 32;
    std::map<std::string, TTF_Font*> fontsCache;
public:
    bool isgame() override { return true; }
    void init() override;
    void clear() override;
    void display() override;
    void drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) override;
    void drawRect(int x, int y, int width, int height, ArcadeColor color) override;
    void drawCircle(int x, int y, int radius, ArcadeColor color) override;
    void drawSprite(int x, int y, const std::string &spritePath) override;
    void drawLine(int x1, int y1, int x2, int y2, ArcadeColor color) override;
    void drawTile(int x, int y, ArcadeColor color) override;
    std::string handleEvents() override;
    std::pair<int,int> getScreenSize() override;
    ~Sdl2Adapter() override;

};

#endif /* !ARCADE_SDL2_HPP_ */
