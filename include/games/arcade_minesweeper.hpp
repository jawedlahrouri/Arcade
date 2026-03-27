/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper
*/

#ifndef ARCADE_1_ARCADE_MINESWEEPER_HPP
#define ARCADE_1_ARCADE_MINESWEEPER_HPP

#include "IGames.hpp"

enum CellType {
    EMPTY = 0,
    BOMB = -1,
};

class arcade_minesweeper : public Arcade::IGames {
    private:
        int _width;                   // Largeur de la grille
        int _height;                  // Hauteur de la grille
        int _difficulty;              // Difficulté actuelle (0, 1, 2)
        int _totalBombs;              // Nombre total de bombes
        int _flagsPlaced;             // Nombre de drapeaux posés
        bool _isGameOver;             // Savoir si le jeu est fini (gagné ou perdu)
        bool _hasWon;                 // Savoir si le joueur a gagné
        bool _firstClick;             // Savoir si le joueur a déjà cliqué une fois
        std::vector<std::vector<int>> _grid;      // La grille contenant la valeur des cases
        std::vector<std::vector<bool>> _revealed; // Grille pour savoir ce qui est cliqué
        std::vector<std::vector<bool>> _flagged;  // Grille pour savoir où sont les drapeaux
        int _cursorX;
        int _cursorY;

    public:
        arcade_minesweeper() = default;
        ~arcade_minesweeper() override = default;

        // Méthodes héritées de Arcade::IGames
        std::any init() override;
        std::any update() override;
        void handleKey(const std::vector<Arcade::InputCommand>& inputs) override;
        void handleMouse(Arcade::MouseEvent inputs) override;
        int getScore() override;
        bool isGameOver() override;
        Arcade::DisplayData getDisplayables() const override;

        // Méthodes utilitaires
        void generate_bomb_field(int safe_x, int safe_y, int difficulty); 
        void reveal_case(int coord_x, int coord_y); 
        void reveal_neighbor_case(int coord_x, int coord_y);
        bool is_bomb(int coord_x, int coord_y);
        void splode_bomb();
        void end_game();
};

#endif //ARCADE_1_ARCADE_MINESWEEPER_HPP