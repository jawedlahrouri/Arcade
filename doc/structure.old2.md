# Scan Complet du Projet ARCADE-1 — 23 Mars 2026 (TEMPORAIRE)

## 1. Vue d'ensemble

ARCADE-1 est un framework de jeux arcade modulaire basé sur une **architecture plugin** avec:

- Un **noyau statique** (`arcade`) qui charge dynamiquement des bibliothèques graphiques et des jeux.
- Des **interface communes** (`IGraphical`, `IGames`) qui définissent les contrats entre plugins.
- Un **système d'événements unifié** (`Arcade::InputCommand`) traversant tous les composants.
- Une **gestion de joueur** (`Player`) pour persister les scores par jeu.

---

## 2. Arborescence complète (texte)

```text
ARCADE-1/ (268 fichiers total, 23 en src/include)
│
├── arcade                          # Exécutable principal généré
├── Makefile                        # Build core + plugins (ncurses/SDL2 + Snake game)
├── README.md                       # README minimal
├── TODO.TXT                        # Checklist patterns / features
│
├── asset/
│   ├── audio/
│   ├── font/
│   │   └── coolvetica/
│   │       ├── Coolvetica Hv Comp.otf
│   │       ├── Coolvetica Rg Cond.otf
│   │       ├── Coolvetica Rg Cram.otf
│   │       ├── Coolvetica Rg It.otf
│   │       └── Coolvetica Rg.otf
│   └── textures/
│
├── doc/
│   ├── STRUCTURE_ACTUELLE.md       # Première doc (patterns en détail)
│   └── ARCADE_SCAN_2026_03_23.md   # Ce document (scan complet)
│
├── include/
│   ├── check_main.hpp              # Validation arguments CLI
│   ├── dll_cmds.hpp                # Template DLLoader<T> pour plugins
│   ├── exception.hpp
│   ├── core/
│   │   ├── core.hpp                # Classe Core (orchestration)
│   │   ├── menu.hpp                # Menu arcade
│   │   └── Player.hpp              # Gestion scores joueur
│   ├── games/
│   │   ├── arcade_snake.hpp        # Header jeu Snake
│   │   └── IGames.hpp              # Interface jeux (Strategy pattern)
│   └── graphicals/
│       ├── IGraphical.hpp            # Interface affichage + enum InputCommand
│       ├── arcade_ncurses.hpp      # Adapter ncurses
│       └── arcade_sdl2.hpp         # Adapter SDL2
│
├── lib/
│   ├── arcade_ncurses.so           # Plugin graphique (ncurses)
│   ├── arcade_sdl2.so              # Plugin graphique (SDL2+TTF)
│   └── arcade_snake.so             # Plugin jeu (Snake)
│
├── src/
│   ├── main.cpp                    # Point d'entrée
│   ├── core/
│   │   ├── Core.cpp                # Boucle principale + orchestration
│   │   ├── menu.cpp                # Logique menu
│   │   └── Player.cpp              # Implémentation scores (squelette)
│   ├── games/
│   │   └── arcade_snake.cpp        # Implémentation Snake
│   └── graphicals/
│       ├── arcade_ncurses.cpp      # Implémentation ncurses
│       └── arcade_sdl2.cpp         # Implémentation SDL2
│
└── tests/
    └── tests_main.cpp              # Placeholder tests
```

---

## 3. Composants principaux et flux

### 3.1. Noyau d'orchestration (`Core`)

**Fichiers:** `include/core/core.hpp`, `src/core/Core.cpp`

**Responsabilités:**

- Charge et initialise le backend graphique (IGraphical) au démarrage.
- Gère la boucle principale avec FPS cap ~60 (16ms par frame).
- Orchestre Menu ↔ Jeu avec transitions fluides.
- Permet le changement de backend graphique en direct (Ctrl+S).

**Flux principal:**

1. Instantiation `Core(libPath)` charge via `DLLoader<IGraphical>::getInstance("createGraphical")`.
2. Boucle `run()`:
   - Récupère les événements unifiés: `std::vector<Arcade::InputCommand>`.
   - En mode Menu: délègue au Menu, qui retourne une `MenuAction`.
   - En mode Jeu: délègue au jeu actuel qui traite les inputs + update + draw.
   - Affiche le rendu via `IGraphical::display()`.

**Clés techniques:**

- `changeGraphicLib(newPath)` décharge l'ancienne lib et charge la nouvelle sans relancer.
- Gestion d'exceptions pour jeux/libs défaillants.

---

### 3.2. Menu arcade (`Menu`)

**Fichiers:** `include/core/menu.hpp`, `src/core/menu.cpp`

**Responsabilités:**

- Affiche liste de jeux et backends graphiques disponibles.
- Gère la navigation avec UP/DOWN, sélection avec ENTER.
- Retourne une `MenuAction {libPath, isGame}` vers le Core.

**Entrée utilisateur (refactorisée):**

- Ancien: `handleInput(std::string event)` → prend 1 événement string.
- **Nouveau:** `handleInput(const std::vector<Arcade::InputCommand>& commands)` → prend vecteur typé.
- Logique: itération sur commands, accumulation des actions.

**État actuel:**

- Liste des libs est **codée en dur** dans `loadLibrariesList()` (TODO: scanner `./lib/`).
- Affichage centré et coloré (YELLOW titre, RED sélection jeu, BLUE sélection graphique).

---

### 3.3. Joueur (`Player`)

**Fichiers:** `include/core/Player.hpp`, `src/core/Player.cpp`

**Responsabilités:**

- Encapsuler données joueur: nom, scores par jeu.
- Méthodes: `getName()`, `getHighScore(gameName)`, `setHighScore(gameName, score)`.
- Persistance: `saveToFile(path)`, `loadFromFile(path)`.

**État actuel:**

- Classe **déclarée avec interface** mais implémentation **squelettique** (getters/setters vides).
- Pas encore intégrée au Core/Menu pour affichage/sauvegarde dynamique.

---

### 3.4. Interfaces de contrat (`IGraphical`, `IGames`)

#### IGraphical (include/graphicals/IGraphical.hpp)

**Définit le contrat pour tout backend d'affichage:**

```cpp
namespace Arcade {
  enum class InputCommand {
    UP, DOWN, LEFT, RIGHT, ENTER,
    SWITCH_GRAPHICS, BACK, QUIT
  };
}

class IGraphical {
  virtual void init() = 0;
  virtual void clear() = 0;
  virtual void display() = 0;
  
  virtual void drawText(int x, int y, const std::string& text, ...) = 0;
  virtual void drawRect(int x, int y, int w, int h, ArcadeColor color) = 0;
  virtual void drawCircle(...) = 0;
  virtual void drawSprite(...) = 0;
  virtual void drawLine(...) = 0;
  virtual void drawTile(int x, int y, ArcadeColor color) = 0;
  
  virtual std::vector<Arcade::InputCommand> pollInputs() = 0;  // Nouveau
  virtual std::pair<int,int> getScreenSize() = 0;
};
```

**Implémentations:**

- `NcursesAdapter`: rendu terminal ncurses, keymap ncurses → Arcade::InputCommand.
- `Sdl2Adapter`: rendu SDL2+TTF 800x600, keymap SDL → Arcade::InputCommand.

#### IGames (include/games/IGames.hpp)

**Définit le contrat pour tout jeu:**

```cpp
class IGames {
  virtual void init(IGraphical& graphicLib) = 0;
  virtual void update() = 0;
  virtual void handleInput(const std::vector<Arcade::InputCommand>& inputs) = 0;
  virtual void draw(IGraphical& graphicLib) = 0;
  virtual bool isGameOver() = 0;
};
```

**Implémentations:**

- `SnakeGame`: squelette (init + affichage titre, logique de gameplay TODO).

---

## 4. Enum et système d'événements

### 4.1. Arcade::InputCommand

**Défini dans:** `include/graphicals/IGraphical.hpp` ligne 14-24

```cpp
namespace Arcade {
  enum class InputCommand {
    NONE,             // Pas d'action (idle)
    UP,               // ↑
    DOWN,             // ↓
    LEFT,             // ←
    RIGHT,            // →
    ENTER,            // Entrée, launch jeu
    SWITCH_GRAPHICS,  // 's'/'S', toggle backend
    BACK,             // Échap, revenir menu
    QUIT              // 'q'/'Q', quitter
  };
}
```

**Mappage clavier:**

- **ncurses** (include/graphicals/arcade_ncurses.hpp:15-26): `KEY_UP`→`UP`, `'s'`→`SWITCH_GRAPHICS`, etc.
- **SDL2** (include/graphicals/arcade_sdl2.hpp:21-27): `SDLK_UP`→`UP`, `SDLK_RETURN`→`ENTER`, etc.

**Flux:** interactions utilisateur → `pollInputs()` → `std::vector<InputCommand>` → Core/Menu/Jeu.

---

## 5. Design patterns utilisés

### Pattern 1: **Plugin + Factory Method** ✅ Actif

**Objectif:** Charger des libs sans dépendre de leurs implémentations.

**Implémentation:**

- `DLLoader<T>` template (include/dll_cmds.hpp:17) utilise `dlopen/dlsym/dlclose`.
- `Core` appelle `getInstance("createGraphical")` (src/core/Core.cpp:16, 31).
- Chaque plugin exporte `extern "C" {...* createGraphical/createGame()}`.

**Fichiers clés:**

| Fichier            | Ligne      | Rôle                          |
| ------------------ | ---------- | ------------------------------ |
| dll_cmds.hpp       | 17, 46     | Template DLLoader, getInstance |
| Core.cpp           | 16, 31, 54 | Appels getInstance             |
| arcade_ncurses.cpp | 187        | export createGraphical         |
| arcade_sdl2.cpp    | 198        | export createGraphical         |
| arcade_snake.cpp   | 42         | export createGame              |

**Impact:** Ajouter un plugin = créer `.so` respectant le contrat, aucune recompilation du core.

---

### Pattern 2: **Strategy** ✅ Actif

**Objectif:** Implémenter plusieurs stratégies (backing graphique, jeu) derrière une même interface.

**Implémentation:**

- `IGraphical` / `IGames` définissent le contrat.
- `NcursesAdapter` / `Sdl2Adapter` / `SnakeGame` varient l'implémentation.
- `Core` manipule l'interface abstraite sans connaître l'implémentation.

**Fichiers:**

| Pattern        | Fichier                       | Ligne                                |
| -------------- | ----------------------------- | ------------------------------------ |
| IGraphical       | graphicals/IGraphical.hpp       | 77                                   |
| IGames         | games/IGames.hpp              | 14                                   |
| NcursesAdapter | graphicals/arcade_ncurses.hpp | 13                                   |
| Sdl2Adapter    | graphicals/arcade_sdl2.hpp    | 17                                   |
| SnakeGame      | games/arcade_snake.hpp        | 13                                   |
| Core orchestre | core/Core.cpp                 | 25 (changeGraphicLib), 46 (loadGame) |

**Impact:** Changement de backend graphique en direct via `changeGraphicLib()`.

---

### Pattern 3: **Adapter** ✅ Actif

**Objectif:** Traduire API externes (ncurses, SDL2) en API projet unifiée.

**Implémentation:**

- `NcursesAdapter` encapsule ncurses derrière `IGraphical`.
- `Sdl2Adapter` encapsule SDL2/TTF derrière `IGraphical`.
- Les jeux et le core appellent `IGraphical` sans connaître ncurses/SDL2.

**Fichiers:**

| Adapter | Header                        | Impl               | Librairie         |
| ------- | ----------------------------- | ------------------ | ----------------- |
| ncurses | graphicals/arcade_ncurses.hpp | arcade_ncurses.cpp | -lncurses         |
| SDL2    | graphicals/arcade_sdl2.hpp    | arcade_sdl2.cpp    | -lSDL2 -lSDL2_ttf |

**Impact:** Abstraire différences entre ncurses/SDL2, jeux transportables.

---

### Pattern 4: **Command** ✅ Actif (remplace Observer)

**Objectif:** Encapsuler actions utilisateur comme des objets / commandes typisées.

**Implémentation (nouvelle):**

- `Arcade::InputCommand` enum (include/graphicals/IGraphical.hpp:15-23) centralise toutes les commandes.
- Chaque backend graphique (`pollInputs()`) exporte les touches brutes en commandes enum.
- `Core`, `Menu`, `IGames` reçoivent `std::vector<Arcade::InputCommand>` et traitent uniformément.
- Mappage clavier:
  - **ncurses** (arcade_ncurses.hpp:15-26): keymap ncurses → `InputCommand`.
  - **SDL2** (arcade_sdl2.hpp:21-30): keymap SDL2 → `InputCommand`.

**Fichiers clés:**

| Fichier            | Ligne   | Rôle                                |
| ------------------ | ------- | ------------------------------------ |
| IGraphical.hpp       | 15-23   | enum InputCommand (`NONE` ajouté) |
| arcade_ncurses.hpp | 15-26   | keymap ncurses                       |
| arcade_sdl2.hpp    | 21-30   | keymap SDL2                          |
| arcade_ncurses.cpp | 127-157 | `pollInputs()` impl                |
| arcade_sdl2.cpp    | 209-226 | `pollInputs()` impl                |
| Core.cpp           | 94-152  | itération sur commands              |
| Menu.cpp           | 41-78   | traite commandes batch               |

**Impact:** Actions fortement typées, versionnables, rejouables, déterministes. Pas d'interprétation de strings.

---

### Pattern 5: **Composition + Injection de Dépendances** ✅ Actif (remplace Singleton)

**Objectif:** Passer les dépendances (IGraphical, Player, etc.) en constructeur plutôt que les demander globalement.

**Implémentation (nouvelle):**

- `Core` reçoit `IGraphical*` et la passe aux jeux: `game->init(*_graphic)`.
- `Player` est construit avec le nom : `Player("PlayerName")`.
- Chaque composant (Menu, Jeu) reçoit ce dont il a besoin plutôt que de chercher une instance globale.
- Pas de `Singleton::GetInstance()`, pas de state global caché.

**Fichiers clés:**

| Fichier    | Ligne   | Rôle                                                     |
| ---------- | ------- | --------------------------------------------------------- |
| core.hpp   | ~20-30  | membres `_graphicLoader`, `_graphic`, `_gameLoader` |
| Core.cpp   | 8-20    | constructeur charge IGraphical                              |
| Core.cpp   | 113-118 | passe IGraphical aux jeux                                   |
| Player.hpp | ~14     | constructeur explicit                                     |
| IGames.hpp | 18      | param `IGraphical& graphicLib` explicite                  |

**Impact:** Dépendances visibles et testables. Structure claire du flux d'execution (pas de magic global). Facilite les tests unitaires et multi-backends.

---

### Pattern 6: **Mediator** ⏳ À explorer (futur)

**Objectif:** Centraliser la communication entre Core, Menu, Jeu, Player pour éviter couplages croisés.

**Scénario potentiel:**

- Un `GameMediator` encapsulerait les interactions Core ↔ Menu ↔ Jeu.
- Transactions: "charge jeu" → mediator → valide → charge → notifie.
- État actuel: logique directe dans Core.cpp, pas de mediator.

**Suivi:** À considérer si complexité de transitions Core/Menu/Jeu augmente.

---

## 6. État fonctionnel

### ✅ Implémenté et actif

- Chargement dynamique plugins (Factory Method).
- Commutation backend graphique en direct (Strategy + Adapter).
- Menu avec navigation UP/DOWN/ENTER/SWITCH_GRAPHICS.
- Enum d'événements unifié (`Arcade::InputCommand`).
- Classe Player (squelette de persistance).

### ⏳ En cours / Squelette

- Jeu Snake: affichage titre, logique de gameplay TODO.
- Player: implémentation sauvegarde/chargement fichier à compléter.
- Menu: liste des libs codée en dur (pas de scan `./lib/`).

### ⏳ En exploration

- Mediator pattern pour orchestration Core/Menu/Jeu complexe.
- Tests unitaires (fichier tests_main.cpp vide).

---

## 7. Dépendances et build

### Compilation

```bash
make              # Compile core + plugins
make clean        # Nettoie .o
make fclean       # Nettoie tout + plugins
make re           # fclean + all
```

### Dépendances externes

- **ncurses** library (`-lncurses`).
- **SDL2 + SDL2_ttf** (`-lSDL2 -lSDL2_ttf`).
- C++20 standard (`-std=gnu++20`).
- JDWP pour debug dynamique.

---

## 8. Exécution

```bash
./arcade ./lib/arcade_ncurses.so    # Lancer avec ncurses
./arcade ./lib/arcade_sdl2.so       # Lancer avec SDL2

# Dans le menu:
#   ↑/↓       : sélectionner jeu
#   ENTER     : lancer jeu sélectionné
#   s/S       : changer backend graphique
#   ESC/q     : revenir menu ou quitter
```

---

## Changements majeurs (Refactorisation patterns)

| Change                            | Détail                                                       |
| --------------------------------- | ------------------------------------------------------------- |
| **IObserver.hpp supprimé** | Ancien pattern Observer remplacé par Command                 |
| **Singleton.hpp supprimé** | Ancien pattern Singleton remplacé par Composition+Injection  |
| **Command pattern actif**   | `Arcade::InputCommand` enum centralise toutes les commandes |
| **Injection dépendances**  | Core, Menu, Jeu reçoivent IGraphical, Player en paramètre     |
| Classe Player ajoutée            | `include/core/Player.hpp`, `src/core/Player.cpp`          |
| IGames::handleInput refactorisé  | `std::string` → `std::vector<Arcade::InputCommand>`      |
| Menu::handleInput refactorisé    | Idem, itération sur commands batch                           |
| pollInputs() refactorisé         | `handleEvents()` → `pollInputs()` + retourne vecteur     |
| Keymap unifié                    | ncurses/SDL2 → Arcade::InputCommand via maps                 |
| InputCommand::NONE ajouté        | Représente absence d'action (idle)                           |

---

## Notes finales

1. **Architecture solide:** Plugin + Factory + Strategy + Adapter + Command + Injection de dépendances = fondement modulaire robuste.
2. **Observer et Singleton remplacés:** Command centralise les événements, Injection remplace les singletons globaux.
3. **Patterns actifs:** Factory Method, Strategy, Adapter, Command, Composition+Injection.
4. **Prochaines étapes:**
   - Compléter implémentation Player (sauvegarde/chargement scores).
   - Implémenter logique Snake complète.
   - Scanner `./lib/` dynamiquement dans Menu.
   - Envisager Mediator si orchestration Core/Menu/Jeu se complexifie.
