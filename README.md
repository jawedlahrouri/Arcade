[![Academic Year](https://img.shields.io/badge/Epitech-Tek2-green?style=for-the-badge&logo=epitech&logoColor=white)](https://www.epitech.eu/)
[![Language: C++20](https://img.shields.io/badge/Language-C%2B%2B20-00599C.svg?style=for-the-badge&logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Module](https://img.shields.io/badge/Module-G--OOP--400-informational?style=for-the-badge)](#)

# 🕹️ Arcade — Plateforme de Jeux Modulaire & Chargement Dynamique en C++20

**Arcade** est une console de jeux vidéo modulaire développée en **C++20**. Son architecture découple strictement le moteur central (Core), les bibliothèques d'affichage graphique et les jeux. Grâce au chargement dynamique de bibliothèques partagées (`.so`) via `dlopen`/`dlsym`, le joueur peut intervertir graphismes et jeux à la volée pendant l'exécution, sans interruption ni recompilation.

---

## 🏛️ Architecture & Interfaces Modulaires

```text
               +-------------------+
               |    Arcade Core    |
               +---------+---------+
                         | (dlopen / dlsym)
         +---------------+---------------+
         |                               |
+--------v--------+             +--------v--------+
| IGraphic (libs) |             |  IGames (jeux)  |
+--------+--------+             +--------+--------+
| - arcade_ncurses|             | - arcade_snake  |
| - arcade_sdl2   |             | - arcade_mines  |
| - arcade_sfml   |             +-----------------+
+-----------------+
```

### 🎮 Jeux Inclus (`IGames`)
- **`arcade_snake.so`** : Le jeu classique du serpent avec gestion de la queue, accélération de la vitesse, génération de fruits et détection de collisions.
- **`arcade_minesweeper.so`** : Démineur complet avec génération aléatoire de mines, placement de drapeaux et révélation récursive des cases vides.

### 🖼️ Bibliothèques Graphiques (`IGraphic`)
- **`arcade_ncurses.so`** : Rendu en mode texte dans le terminal via Ncurses.
- **`arcade_sdl2.so`** : Rendu graphique 2D avec SDL2.
- **`arcade_sfml.so`** : Rendu graphique fenêtré matériellement accéléré avec SFML.

---

## ⌨️ Raccourcis Clavier en Cours de Jeu

| Touche | Action |
|---|---|
| `2` / `3` | Basculer vers la bibliothèque graphique précédente / suivante. |
| `4` / `5` | Basculer vers le jeu précédent / suivant. |
| `R` | Redémarrer la partie en cours. |
| `M` | Retourner au menu principal d'Arcade. |
| `Échap` | Quitter l'application. |

---

## 🛠️ Compilation & Lancement

```bash
make            # Compile le Core (arcade), les bibliothèques graphiques et les jeux
make core       # Compile uniquement le binaire principal
make games      # Compile l'ensemble des jeux (.so)
make graphicals # Compile l'ensemble des bibliothèques graphiques (.so)
make clean      # Nettoie les fichiers intermédiaires
make fclean     # Supprime tous les binaires et bibliothèques .so
make re         # Recompile l'intégralité

# Lancement avec la bibliothèque graphique initiale de votre choix
./arcade ./lib/arcade_sfml.so
# ou en mode console :
./arcade ./lib/arcade_ncurses.so
```
