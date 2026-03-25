/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_sdl2
*/

#include "./graphicals/arcade_sdl2.hpp"
#include "exception.hpp"
#include <SDL2/SDL_keycode.h>
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
    {SDLK_r, Arcade::InputCommand::RELOAD_GAME},
    {SDLK_n, Arcade::InputCommand::SWITCH_GAME},
    {SDLK_ESCAPE, Arcade::InputCommand::BACK},
    {SDLK_SPACE, Arcade::InputCommand::FLAG},
    {SDLK_q, Arcade::InputCommand::QUIT}
};

/**
 * @brief Initializes the asset manager with a valid SDL renderer.
 * @param renderer Renderer used to create textures.
 * @return Does not return a value.
 */
Sdl2AssetManager::Sdl2AssetManager(SDL_Renderer* renderer) : _renderer(renderer)
{
}

/**
 * @brief Releases every loaded texture and clears the internal cache.
 * @return Does not return a value.
 */
Sdl2AssetManager::~Sdl2AssetManager()
{
    for (auto& entry : _textures) {
        if (entry.second != nullptr) {
            SDL_DestroyTexture(entry.second);
        }
    }
    _textures.clear();
}

/**
 * @brief Retrieves a texture from cache or loads it from disk.
 * @param path Path to the BMP file to load.
 * @return Pointer to the SDL texture, or nullptr on failure.
 */
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

/**
 * @brief Initializes SDL, SDL_ttf, the window, renderer, and asset manager.
 * @return Does not return a value.
 */
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

/**
 * @brief Clears the current frame by filling it with black.
 * @return Does not return a value.
 */
void Sdl2Adapter::clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/**
 * @brief Draws a filled tile at grid coordinates.
 * @param x Horizontal position in tiles.
 * @param y Vertical position in tiles.
 * @param color Fill color of the tile.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawTile(int x, int y, Arcade::Color color)
{
    SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
    switch (color) {
        case Arcade::Color::BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
        case Arcade::Color::RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
        case Arcade::Color::GREEN: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
        case Arcade::Color::YELLOW: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
        case Arcade::Color::BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
        case Arcade::Color::MAGENTA: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
        case Arcade::Color::CYAN: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
        case Arcade::Color::WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
    }
    SDL_RenderFillRect(renderer, &rect);
}

/**
 * @brief Renders text with the requested font and draws it on the grid.
 * @param x Horizontal position in tiles.
 * @param y Vertical position in tiles.
 * @param text Text content to draw.
 * @param color Text color.
 * @param fontPath Path to the font file.
 * @param fontSize Font size in pixels.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath, int fontSize)
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
        case Arcade::Color::BLACK: sdlColor = {0, 0, 0, 255}; break;
        case Arcade::Color::RED: sdlColor = {255, 0, 0, 255}; break;
        case Arcade::Color::GREEN: sdlColor = {0, 255, 0, 255}; break;
        case Arcade::Color::YELLOW: sdlColor = {255, 255, 0, 255}; break;
        case Arcade::Color::BLUE: sdlColor = {0, 0, 255, 255}; break;
        case Arcade::Color::MAGENTA: sdlColor = {255, 0, 255, 255}; break;
        case Arcade::Color::CYAN: sdlColor = {0, 255, 255, 255}; break;
        case Arcade::Color::WHITE: sdlColor = {255, 255, 255, 255}; break;
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

/**
 * @brief Measures text width and converts it to tile units.
 * @param text Text to measure.
 * @param fontPath Path to the font file.
 * @param fontSize Font size in pixels.
 * @return Text width in tiles, with fallback approximation on error.
 */
int Sdl2Adapter::measureTextWidth(const std::string &text, const std::string &fontPath, int fontSize)
{
    std::string key = fontPath + ":" + std::to_string(fontSize);
    if (fontsCache.find(key) == fontsCache.end()) {
        TTF_Font* newFont = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!newFont) {
            return static_cast<int>((text.length() + 1) / 2);
        }
        fontsCache[key] = newFont;
    }

    TTF_Font* font = fontsCache[key];
    int textPxWidth = 0;
    int textPxHeight = 0;
    if (TTF_SizeText(font, text.c_str(), &textPxWidth, &textPxHeight) != 0) {
        (void)textPxHeight;
        return static_cast<int>((text.length() + 1) / 2);
    }
    (void)textPxHeight;
    return (textPxWidth + tileSize - 1) / tileSize;
}

/**
 * @brief Presents the current render buffer on screen.
 * @return Does not return a value.
 */
void Sdl2Adapter::display()
{
    SDL_RenderPresent(renderer);
}

/**
 * @brief Returns the current window size in tile units.
 * @return Pair containing width and height in tiles.
 */
std::pair<int,int> Sdl2Adapter::getScreenSize()
{
    int w = 0, h = 0;
    if (window) {
        SDL_GetWindowSize(window, &w, &h);
    }
    return { w / tileSize, h / tileSize };
}

/**
 * @brief Applies an Arcade color converted to SDL draw color.
 * @param renderer Target renderer.
 * @param color Arcade color value to convert.
 * @return Does not return a value.
 */
static void sdl_set_color(SDL_Renderer* renderer, Arcade::Color color)
{
    switch (color) {
        case Arcade::Color::BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
        case Arcade::Color::RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
        case Arcade::Color::GREEN: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
        case Arcade::Color::YELLOW: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;
        case Arcade::Color::BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
        case Arcade::Color::MAGENTA: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break;
        case Arcade::Color::CYAN: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;
        case Arcade::Color::WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
    }
}

/**
 * @brief Draws a rectangle outline using grid coordinates.
 * @param x Rectangle left position in tiles.
 * @param y Rectangle top position in tiles.
 * @param width Rectangle width in tiles.
 * @param height Rectangle height in tiles.
 * @param color Outline color.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawRect(int x, int y, int width, int height, Arcade::Color color)
{
    SDL_Rect rect = {x * tileSize, y * tileSize, width * tileSize, height * tileSize};
    sdl_set_color(renderer, color);
    SDL_RenderDrawRect(renderer, &rect);
}

/**
 * @brief Draws a filled circle centered on grid coordinates.
 * @param x Horizontal center position in tiles.
 * @param y Vertical center position in tiles.
 * @param radius Circle radius in tiles.
 * @param color Circle color.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawCircle(int x, int y, int radius, Arcade::Color color)
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

/**
 * @brief Loads then draws a sprite at the requested position.
 * @param x Horizontal position in tiles.
 * @param y Vertical position in tiles.
 * @param spritePath Path to the BMP sprite file.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawSprite(int x, int y, const std::string &spritePath)
{
    if (!assetManager) {
        return;
    }

    SDL_Texture* texture = assetManager->getTexture(spritePath);
    if (!texture) {
        return;
    }

    SDL_Rect dstRect = {x * tileSize, y * tileSize, tileSize, tileSize};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

/**
 * @brief Draws a line between two points expressed in grid units.
 * @param x1 Start point X coordinate.
 * @param y1 Start point Y coordinate.
 * @param x2 End point X coordinate.
 * @param y2 End point Y coordinate.
 * @param color Line color.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawLine(int x1, int y1, int x2, int y2, Arcade::Color color)
{
    sdl_set_color(renderer, color);
    SDL_RenderDrawLine(renderer, x1 * tileSize, y1 * tileSize, x2 * tileSize, y2 * tileSize);
}

/**
 * @brief Draws the outline of a single tile at the given position.
 * @param x Horizontal position in tiles.
 * @param y Vertical position in tiles.
 * @param color Outline color.
 * @return Does not return a value.
 */
void Sdl2Adapter::drawbox(int x, int y, Arcade::Color color)
{
    sdl_set_color(renderer, color);
    SDL_Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize};
    SDL_RenderDrawRect(renderer, &rect);
}

/**
 * @brief Draws a list of sprite objects through a texture lookup table.
 * @param objects Logical sprite objects to render.
 * @return Does not return a value.
 */
void Sdl2Adapter::draw(const std::vector<Arcade::SpriteObject> &objects)
{
    if (!assetManager) {
        return;
    }

    static const std::map<Arcade::ElementType, std::string> texturePathMap = {
        {Arcade::ElementType::WALL, "asset/textures/wall.png"},
        {Arcade::ElementType::PLAYER_UP, "asset/textures/player_up.bmp"},
        {Arcade::ElementType::PLAYER_DOWN, "asset/textures/player_down.bmp"},
        {Arcade::ElementType::PLAYER_LEFT, "asset/textures/player_left.bmp"},
        {Arcade::ElementType::PLAYER_RIGHT, "asset/textures/player_right.bmp"},
        {Arcade::ElementType::FOOD, "asset/textures/food.bmp"}
    };

    for (const auto &obj : objects) {
        const auto texIt = texturePathMap.find(obj.type);
        if (texIt == texturePathMap.end()) {
            continue;
        }

        SDL_Texture *texture = assetManager->getTexture(texIt->second);
        if (!texture) {
            continue;
        }

        SDL_Rect dstRect = {obj.x * tileSize, obj.y * tileSize, tileSize, tileSize};
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    }
}

/**
 * @brief Draws sprites first, then text entries from display data.
 * @param data Render data that groups sprites and texts.
 * @return Does not return a value.
 */
void Sdl2Adapter::draw(const Arcade::DisplayData &data)
{
    static const std::string defaultFont = "asset/font/coolvetica/Coolvetica Rg.otf";

    draw(data.sprites);
    for (const auto &text : data.texts) {
        drawText(text.x, text.y, text.text, text.color, defaultFont, 24);
    }
}

/**
 * @brief Polls SDL events and converts them to Arcade commands.
 * @return Detected command list, or NONE when no input is available.
 */
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

/**
 * @brief Releases allocated SDL resources, fonts, and cached assets.
 * @return Does not return a value.
 */
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

/**
 * @brief Dynamically creates an instance of the SDL2 graphic module.
 * @return Pointer to a newly allocated IGraphic instance.
 */
extern "C" IGraphic* createGraphical() {
    return new Sdl2Adapter();
}