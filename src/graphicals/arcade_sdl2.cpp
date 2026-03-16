/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sdl2
*/

#include "./graphicals/arcade_sdl2.hpp"
#include "exception.hpp"
#include <iostream>
#include <ostream>

void Sdl2Adapter::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Exception("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }
    window = SDL_CreateWindow("Arcade - SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        throw Exception("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw Exception("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

void Sdl2Adapter::clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Sdl2Adapter::drawTile(int x, int y, ArcadeColor color)
{
    SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
    switch (color) {
        case ArcadeColor::BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
        case ArcadeColor::RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
        case ArcadeColor::GREEN: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
        case ArcadeColor::YELLOW: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
        case ArcadeColor::BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
        case ArcadeColor::MAGENTA: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
        case ArcadeColor::CYAN: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
        case ArcadeColor::WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
    }
    SDL_RenderFillRect(renderer, &rect);
}

void Sdl2Adapter::drawText(int x, int y, const std::string &text, ArcadeColor color)
{
    drawTile(x, y, color);
}

void Sdl2Adapter::display()
{
    SDL_RenderPresent(renderer);
}

static void sdl_set_color(SDL_Renderer* renderer, ArcadeColor color)
{
    switch (color) {
        case ArcadeColor::BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
        case ArcadeColor::RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
        case ArcadeColor::GREEN: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
        case ArcadeColor::YELLOW: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
        case ArcadeColor::BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
        case ArcadeColor::MAGENTA: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
        case ArcadeColor::CYAN: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
        case ArcadeColor::WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
    }
}

void Sdl2Adapter::drawRect(int x, int y, int width, int height, ArcadeColor color)
{
    SDL_Rect rect = {x * tileSize, y * tileSize, width * tileSize, height * tileSize};
    sdl_set_color(renderer, color);
    SDL_RenderFillRect(renderer, &rect);
}

void Sdl2Adapter::drawCircle(int x, int y, int radius, ArcadeColor color)
{
    sdl_set_color(renderer, color);
    int cx = x * tileSize + tileSize / 2;
    int cy = y * tileSize + tileSize / 2;
    for (int w = 0; w < radius * tileSize * 2; w++) {
        for (int h = 0; h < radius * tileSize * 2; h++) {
            int dx = radius * tileSize - w;
            int dy = radius * tileSize - h;
            if ((dx*dx + dy*dy) <= (radius * tileSize) * (radius * tileSize)) {
                SDL_RenderDrawPoint(renderer, cx + dx - radius * tileSize, cy + dy - radius * tileSize);
            }
        }
    }
}

void Sdl2Adapter::drawSprite(int x, int y, const std::string &spritePath)
{
    (void)x; (void)y; (void)spritePath;
}

void Sdl2Adapter::drawLine(int x1, int y1, int x2, int y2, ArcadeColor color)
{
    sdl_set_color(renderer, color);
    SDL_RenderDrawLine(renderer, x1 * tileSize, y1 * tileSize, x2 * tileSize, y2 * tileSize);
}

void Sdl2Adapter::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            throw Exception("SDL window closed");
        } else if (event.type == SDL_KEYDOWN) {
            std::cout << "Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
        } else if (event.type == SDL_KEYUP) {
            std::cout << "Key released: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
        }
    }
}

Sdl2Adapter::~Sdl2Adapter()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
} 

extern "C" IGraphic* createGraphical() {
    return new Sdl2Adapter();
}