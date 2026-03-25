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
    /**
     * @brief Builds the asset manager for a given SDL renderer.
     * @param renderer Renderer used to create textures.
     * @return Does not return a value.
     */
    explicit Sdl2AssetManager(SDL_Renderer* renderer);
    /**
     * @brief Releases all cached textures.
     * @return Does not return a value.
     */
    ~Sdl2AssetManager();

    /**
     * @brief Returns a cached texture or loads it from disk.
     * @param path Path to the texture file.
     * @return Pointer to SDL texture or nullptr on failure.
     */
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
    /**
     * @brief Initializes SDL2 resources for rendering.
     * @return Does not return a value.
     */
    void init() override;
    /**
     * @brief Clears the current render target.
     * @return Does not return a value.
     */
    void clear() override;
    /**
     * @brief Presents the rendered frame on screen.
     * @return Does not return a value.
     */
    void display() override;
    /**
     * @brief Draws text with a selected font and color.
     * @param x Text X position.
     * @param y Text Y position.
     * @param text Text content.
     * @param color Text color.
     * @param fontPath Path to the font file.
     * @param fontSize Font size in pixels.
     * @return Does not return a value.
     */
    void drawText(int x, int y, const std::string &text, Arcade::Color color, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) override;
    /**
     * @brief Measures text width in tile units.
     * @param text Text content.
     * @param fontPath Path to the font file.
     * @param fontSize Font size in pixels.
     * @return Measured width in tiles.
     */
    int measureTextWidth(const std::string &text, const std::string &fontPath = "asset/font/coolvetica/Coolvetica Rg.otf", int fontSize = 24) override;
    /**
     * @brief Draws a rectangle border.
     * @param x Rectangle X position.
     * @param y Rectangle Y position.
     * @param width Rectangle width.
     * @param height Rectangle height.
     * @param color Rectangle color.
     * @return Does not return a value.
     */
    void drawRect(int x, int y, int width, int height, Arcade::Color color) override;
    /**
     * @brief Draws a filled circle.
     * @param x Circle center X position.
     * @param y Circle center Y position.
     * @param radius Circle radius.
     * @param color Circle color.
     * @return Does not return a value.
     */
    void drawCircle(int x, int y, int radius, Arcade::Color color) override;
    /**
     * @brief Draws a sprite loaded from a path.
     * @param x Sprite X position.
     * @param y Sprite Y position.
     * @param spritePath Path to sprite resource.
     * @return Does not return a value.
     */
    void drawSprite(int x, int y, const std::string &spritePath) override;
    /**
     * @brief Draws a line between two points.
     * @param x1 Start X position.
     * @param y1 Start Y position.
     * @param x2 End X position.
     * @param y2 End Y position.
     * @param color Line color.
     * @return Does not return a value.
     */
    void drawLine(int x1, int y1, int x2, int y2, Arcade::Color color) override;
    /**
     * @brief Draws a filled tile.
     * @param x Tile X position.
     * @param y Tile Y position.
     * @param color Tile color.
     * @return Does not return a value.
     */
    void drawTile(int x, int y, Arcade::Color color) override;
    /**
     * @brief Draws a single box tile border.
     * @param x Tile X position.
     * @param y Tile Y position.
     * @param color Tile color.
     * @return Does not return a value.
     */
    void drawbox(int x, int y, Arcade::Color color) override;
    /**
     * @brief Draws a list of sprite objects.
     * @param objects Sprite objects to render.
     * @return Does not return a value.
     */
    void draw(const std::vector<Arcade::SpriteObject> &objects) override;
    /**
     * @brief Draws complete display data for one frame.
     * @param data Display data to render.
     * @return Does not return a value.
     */
    void draw(const Arcade::DisplayData &data) override;
    /**
     * @brief Polls SDL events and maps them to Arcade commands.
     * @return Vector of input commands.
     */
    std::vector<Arcade::InputCommand> pollInputs() override;
    /**
     * @brief Returns the current screen size in logical units.
     * @return Pair of width and height.
     */
    std::pair<int,int> getScreenSize() override;
    /**
     * @brief Releases SDL2 resources and cached assets.
     * @return Does not return a value.
     */
    ~Sdl2Adapter() override;

};

#endif /* !ARCADE_SDL2_HPP_ */
