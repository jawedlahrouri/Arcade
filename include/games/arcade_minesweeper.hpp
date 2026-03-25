/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** arcade_minesweeper
*/

#ifndef ARCADE_1_ARCADE_MINESWEEPER_HPP
#define ARCADE_1_ARCADE_MINESWEEPER_HPP

#include "IGames.hpp"

typedef struct coordonate_s {
    int coord_x;
    int coord_y;
} coordonate_t;

enum CellType {
    EMPTY = 0,
    BOMB = -1,
};

class arcade_minesweeper : public IGames {
    private:
        int _width;                   // Largeur de la grille
        int _height;                  // Hauteur de la grille
        int _difficulty;              // Difficulté actuelle (0, 1, 2)
        int _totalBombs;              // Nombre total de bombes
        bool _isGameOver;             // Savoir si le jeu est fini (gagné ou perdu)
        bool _hasWon;                 // Savoir si le joueur a gagné
        std::vector<std::vector<int>> _grid;     // La grille contenant la valeur des cases (bombes/nombres)
        std::vector<std::vector<bool>> _revealed; // Grille pour savoir ce qui est cliqué
        std::vector<std::vector<bool>> _flagged;  // Grille pour savoir où sont les drapeaux
        // Éléments à envoyer à IGraphic
        std::vector<Arcade::SpriteObject> _displayables;
        int _cursorX;
        int _cursorY;

    public:
        // Méthodes héritées de IGames
        void init() override;
        void update() override;
        void handleInput(const std::vector<Arcade::InputCommand>& inputs) override;
        bool isGameOver() override;
        std::vector<Arcade::SpriteObject> getDisplayables() const override;

        // Méthodes liés à la génération de la grille
        void create_starting_zone(); // créer la zone large au debut du demineur
        std::vector<std::vector<int>> generate_bomb_field(int difficulty); // generes le champ de bombe
        int change_difficulty(int counter); // %3 du nombre si 0 c facile 1 moyen 2 difficile

        // Méthodes liés à l'interception des entrées du joueur
        int get_px(int coord_x); // recuperes la position x du clic
        int get_py(int coord_y); //recuperes la position y du clic

        // Méthodes lié à la mise à jour de la grille et aux différents event in-game
        int count_bomb(int coord_x, int coord_y); //compte le nombre de bombe tout  autour 
        void reveal_case(int coord_x, int coord_y); // reveal les cases sur la carte
        int remove_empty(int coord_x, int coord_y, int **field); // enleves toutes les cases "isolé" collé à celle sur laquelle nous venons de tomber
        void reveal_neighbor_case(int coord_x, int coord_y); // reveal les cases voisine à celle donné et affiche le nombre de bombe autour de celles-ci

        // Méthodes pour vérifier le contenu d'une case
        bool is_bomb(int coord_x, int coord_y);  // regarde si la case sur laquelle nous venons de cliquer n'est pas une bombe
        bool is_reveal(int coord_x, int coord_y); // regarde si la case sur laquelle nous cliquons est déjà reveal ou non
        bool is_flag(int coord_x, int coord_y); // verifie si la case cliquer est flag ou pas
        
        // Méthodes pour gérer les events récurrent de la partie
        void splode_bomb(); // fini la partie dans le cas ou tu touches une bombe
        void end_game(); // fini la partie si toutes les bombes sont flag
        int count_field(); // compte le nombre de bombe presente dans tout le champ
        
    };


#endif //ARCADE_1_ARCADE_MINESWEEPER_HPP