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
#include <vector>

const std::unordered_map<SDL_Keycode, Arcade::InputCommand> Sdl2Adapter::KEY_MAP = {
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

Sdl2AssetManager::Sdl2AssetManager(SDL_Renderer* renderer) : _renderer(renderer)
{
}

Sdl2AssetManager::~Sdl2AssetManager()
{
    for (auto& entry : _textures) {
        if (entry.second != nullptr) {
            SDL_DestroyTexture(entry.second);
        }
    }
    _textures.clear();
}

SDL_Texture* Sdl2AssetManager::getTexture(const std::string& path)
{
    auto found = _textures.find(path);
    if (found != _textures.end()) {
        return found->second;
    }

    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (surface == nullptr) {
        std::cerr << "Failed to load sprite '" << path << "': " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture from '" << path << "': " << SDL_GetError() << std::endl;
        return nullptr;
    }

    _textures[path] = texture;
    return texture;
}

void Sdl2Adapter::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Exception("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }
    if (TTF_Init() == -1) {
        throw Exception("TTF could not initialize! TTF_Error: " + std::string(TTF_GetError()));
    }
    window = SDL_CreateWindow("Arcade - SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        throw Exception("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw Exception("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
    assetManager = std::make_unique<Sdl2AssetManager>(renderer);
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

void Sdl2Adapter::drawText(int x, int y, const std::string &text, ArcadeColor color, const std::string &fontPath, int fontSize)
{
    std::string key = fontPath + ":" + std::to_string(fontSize);
    if (fontsCache.find(key) == fontsCache.end()) {
        TTF_Font* newFont = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!newFont) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }
        fontsCache[key] = newFont;
    }

    TTF_Font* font = fontsCache[key];
    SDL_Color sdlColor = {255, 255, 255, 255};
    switch (color) {
        case ArcadeColor::BLACK: sdlColor = {0, 0, 0, 255}; break;
        case ArcadeColor::RED: sdlColor = {255, 0, 0, 255}; break;
        case ArcadeColor::GREEN: sdlColor = {0, 255, 0, 255}; break;
        case ArcadeColor::YELLOW: sdlColor = {255, 255, 0, 255}; break;
        case ArcadeColor::BLUE: sdlColor = {0, 0, 255, 255}; break;
        case ArcadeColor::MAGENTA: sdlColor = {255, 0, 255, 255}; break;
        case ArcadeColor::CYAN: sdlColor = {0, 255, 255, 255}; break;
        case ArcadeColor::WHITE: sdlColor = {255, 255, 255, 255}; break;
    }

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
    if (!surfaceMessage) return;
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (!Message) {
        SDL_FreeSurface(surfaceMessage);
        return;
    }

    SDL_Rect Message_rect;
    Message_rect.x = x * tileSize;
    Message_rect.y = y * tileSize;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void Sdl2Adapter::display()
{
    SDL_RenderPresent(renderer);
}

std::pair<int,int> Sdl2Adapter::getScreenSize()
{
    int w = 0, h = 0;
    if (window) {
        SDL_GetWindowSize(window, &w, &h);
    }
    return { w / tileSize, h / tileSize };
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
    if (!assetManager) {
        return;
    }

    SDL_Texture* texture = assetManager->getTexture(spritePath);
    if (texture == nullptr) {
        return;
    }

    SDL_Rect dstRect = {x * tileSize, y * tileSize, tileSize, tileSize};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void Sdl2Adapter::drawLine(int x1, int y1, int x2, int y2, ArcadeColor color)
{
    sdl_set_color(renderer, color);
    SDL_RenderDrawLine(renderer, x1 * tileSize, y1 * tileSize, x2 * tileSize, y2 * tileSize);
}

std::vector<Arcade::InputCommand> Sdl2Adapter::pollInputs()
{
    std::vector<Arcade::InputCommand> commands;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            commands.push_back(Arcade::InputCommand::QUIT);
        } else if (event.type == SDL_KEYDOWN) {
            const auto mapped = Sdl2Adapter::KEY_MAP.find(event.key.keysym.sym);
            if (mapped != Sdl2Adapter::KEY_MAP.end()) {
                commands.push_back(mapped->second);
            }
        }
    }
    if (commands.empty()) {
        commands.push_back(Arcade::InputCommand::NONE);
    }
    return commands;
}

Sdl2Adapter::~Sdl2Adapter()
{
    assetManager.reset();

    for (auto& pair : fontsCache) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    fontsCache.clear();
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
} 

extern "C" IGraphic* createGraphical() {
    return new Sdl2Adapter();
}