# Scan Complet du Projet ARCADE-1 — 25 Mars 2026 (Mise a jour)

## 1. Vue d'ensemble

ARCADE-1 est un framework de jeux arcade modulaire base sur une architecture plugin avec:

- Un noyau statique (`arcade`) qui charge dynamiquement des bibliotheques graphiques et des jeux.
- Des interfaces communes (`IGraphic`, `IGames`) qui definissent les contrats entre plugins.
- Un systeme d'entrees unifie (`Arcade::InputCommand`) partage entre le core, le menu et les jeux.
- Une base de gestion joueur (`Player`) prete pour la persistance des scores (implementation encore partielle).

---

## 2. Arborescence actuelle (texte)

```text
ARCADE-1/
│
├── arcade                          # Executable principal (genere par make)
├── Makefile                        # Build du core + plugins + tests
├── README.md
├── TODO.TXT
│
├── asset/
│   ├── audio/
│   ├── font/
│   │   └── coolvetica/
│   └── textures/
│
├── doc/
│   ├── .gitkeep
│   ├── structure.old.md
│   ├── structure.old2.md
│   └── structure.current.md        # Ce document
│
├── include/
│   ├── check_main.hpp              # Validation arguments CLI
│   ├── dll_cmds.hpp                # Template DLLoader<T> (dlopen/dlsym)
│   ├── exception.hpp
│   ├── core/
│   │   ├── core.hpp                # Classe Core (orchestration)
│   │   ├── menu.hpp                # Menu + actions de selection
│   │   └── Player.hpp              # Profil joueur + highscores
│   ├── games/
│   │   ├── IGames.hpp              # Contrat plugin jeu
│   │   └── arcade_snake.hpp        # Header SnakeGame
│   └── graphicals/
│       ├── IGraphic.hpp            # Contrat plugin graphique + types communs
│       ├── arcade_ncurses.hpp      # Adapter ncurses
│       └── arcade_sdl2.hpp         # Adapter SDL2
│
├── lib/                            # Cree par make, supprime par make fclean
│   ├── arcade_ncurses.so
│   ├── arcade_sdl2.so
│   └── arcade_snake.so
│
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Core.cpp
│   │   ├── menu.cpp
│   │   └── Player.cpp
│   ├── games/
│   │   └── arcade_snake.cpp
│   └── graphicals/
│       ├── arcade_ncurses.cpp
│       └── arcade_sdl2.cpp
│
└── tests/
    └── tests_main.cpp
```

---

## 3. Composants principaux et flux

### 3.1. Noyau d'orchestration (`Core`)

**Fichiers:** `include/core/core.hpp`, `src/core/Core.cpp`

**Responsabilites:**

- Charge et initialise une lib graphique au demarrage.
- Cadence la boucle de jeu avec un pas minimum d'environ 16 ms (
  environ 60 FPS).
- Gere les transitions Menu <-> Jeu.
- Gere le switch de lib graphique a chaud.

**Flux principal (`run`) :**

1. Initialisation de la lib graphique active (`_graphic->init()`).
2. Chargement de la liste des libs via `Menu::loadLibrariesList()`.
3. Boucle principale:
   - Poll des commandes (`pollInputs`).
   - Traitement des commandes globales (`QUIT`, `BACK`, `SWITCH_GRAPHICS`).
   - Rendu du menu via `DisplayData` ou rendu du jeu via `SpriteObject`.
   - Affichage final (`display`).

**Points techniques:**

- `changeGraphicLib(...)` recree loader + instance graphique proprement.
- `loadGame(...)`/`unloadGame()` encapsulent le cycle de vie des jeux.
- En cas d'exception frame, le core log l'erreur et stoppe la boucle.

---

### 3.2. Menu (`Menu`)

**Fichiers:** `include/core/menu.hpp`, `src/core/menu.cpp`

**Responsabilites:**

- Scanner `./lib` pour decouvrir automatiquement les `.so`.
- Determiner le type de plugin (`GAME` ou `GRAPHIC`) via symboles exportes.
- Gérer la navigation (UP/DOWN), lancement jeu (ENTER), switch graphique (`SWITCH_GRAPHICS`).
- Produire un rendu semantique via `Arcade::DisplayData`.

**Etat actuel:**

- Le scan est dynamique (plus de liste hardcodee).
- La sortie visuelle contient bordures + titres + listes + warnings si libs manquantes.

---

### 3.3. Joueur (`Player`)

**Fichiers:** `include/core/Player.hpp`, `src/core/Player.cpp`

**Responsabilites visees:**

- Stocker le nom et les highscores par jeu.
- Sauvegarder/charger depuis un fichier.

**Etat actuel:**

- Interface complete en header.
- Implementation encore minimale/squelette dans `Player.cpp` (logique metier a finaliser).

---

### 3.4. Contrats de plugins (`IGraphic`, `IGames`)

#### `IGraphic` (`include/graphicals/IGraphic.hpp`)

Expose:

- Initialisation et cycle de rendu: `init`, `clear`, `display`.
- Primitives de dessin: `drawText`, `drawRect`, `drawCircle`, `drawSprite`, `drawLine`, `drawTile`, `drawbox`.
- Dessin par donnees semantiques: `draw(vector<SpriteObject>)` et `draw(DisplayData)`.
- Entrees et viewport: `pollInputs`, `getScreenSize`.

Types communs dans `namespace Arcade`:

- `InputCommand`: `NONE`, `UP`, `DOWN`, `LEFT`, `RIGHT`, `ENTER`, `SWITCH_GRAPHICS`, `BACK`, `QUIT`.
- `Color`, `ElementType`, `SpriteObject`, `TextObject`, `DisplayData`.

#### `IGames` (`include/games/IGames.hpp`)

Expose:

- `init()`
- `update()`
- `handleInput(const std::vector<Arcade::InputCommand>&)`
- `isGameOver()`
- `getDisplayables() const` (retourne les sprites du frame courant)

**Implementation actuelle:**

- `SnakeGame` (squelette) dans `src/games/arcade_snake.cpp`.

---

## 4. Mapping des entrees

### 4.1. Commandes unifiees

- Les backends graphiques convertissent les evenements natifs en `Arcade::InputCommand`.
- Le core traite ensuite ces commandes de facon uniforme.

### 4.2. Actions globales du core

- `QUIT`:
  - en menu: sortie application,
  - en jeu: retour menu.
- `BACK`:
  - en jeu: retour menu.
- `SWITCH_GRAPHICS`:
  - passe au backend graphique suivant via le menu.

---

## 5. Plugins et points d'entree dynamiques

- Plugin graphique: symbole attendu `createGraphical`.
- Plugin jeu: symbole attendu `createGame`.
- `DLLoader<T>` ouvre la lib (`dlopen`), resolve le symbole (`dlsym`) puis instancie l'objet.

Plugins actuels:

- Graphiques: `arcade_ncurses.so`, `arcade_sdl2.so`
- Jeux: `arcade_snake.so`

---

## 6. Patterns utilises (etat reel)

### 6.1. Plugin + Factory Method (actif)

- Chargement runtime des plugins sans liaison compilee forte.
- Base sur `DLLoader<T>` + symboles `createGraphical` / `createGame`.

### 6.2. Strategy (actif)

- Le core manipule `IGraphic` et `IGames`.
- Les implementations concretes sont interchangeables (`NcursesAdapter`, `Sdl2Adapter`, `SnakeGame`).

### 6.3. Adapter (actif)

- Chaque backend traduit son API native (ncurses / SDL2) vers le contrat `IGraphic`.

### 6.4. Command (actif)

- Les interactions utilisateur sont normalisees via `Arcade::InputCommand`.

### 6.5. Composition / DI legere (partiellement active)

- `Core` compose ses dependances dynamiques (`_graphic`, `_currentGame`, loaders).
- Les objets sont crees explicitement et geres par ownership (`unique_ptr`).

### 6.6. Observer / Singleton

- Non utilises dans le flux principal actuel.

---

## 7. Build et execution

### 7.1. Compilation

```bash
make
make clean
make fclean
make re
```

### 7.2. Cibles generees

- Executable: `arcade`
- Plugins: `lib/arcade_ncurses.so`, `lib/arcade_sdl2.so`, `lib/arcade_snake.so`

### 7.3. Dependances externes

- `-ldl`
- `-lncurses`
- `-lSDL2 -lSDL2_ttf`
- Standard C++20 (`-std=gnu++20`)

### 7.4. Lancement

```bash
./arcade ./lib/arcade_ncurses.so
./arcade ./lib/arcade_sdl2.so
```

---

## 8. Etat fonctionnel (resume)

### Implementé et actif

- Chargement dynamique plugins.
- Menu avec scan auto de `./lib`.
- Changement de backend graphique a chaud.
- Boucle core stable avec cadence de frame.

### Partiellement implementé

- `SnakeGame`: logique jeu basique/squelette.
- `Player`: persistance et gestion score a completer.

### Pistes suivantes

1. Finaliser gameplay Snake (deplacement, collisions, score).
2. Integrer `Player` dans le cycle Core/Menu (chargement/sauvegarde, affichage score).
3. Ajouter tests unitaires pour menu, core et loader dynamique.
