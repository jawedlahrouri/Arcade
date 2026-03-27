# Structure actuelle du projet ARCADE-1 (TEMPORAIRE !!!!!) v1 23/03/26

Ce document décrit l'etat actuel du depot, son organisation technique, et le flux d'execution principal.

## 1) Vue d'ensemble

Le projet est structure autour d'un executable coeur (`arcade`) qui charge dynamiquement des bibliotheques partagees (`.so`) pour:

- l'affichage (graphical plugins),
- les jeux (game plugins).

Le chargement dynamique repose sur des interfaces C++ communes (`IGraphic`, `IGames`) et des points d'entree C (`createGraphical`, `createGame`).

## 2) Arborescence actuelle (texte)

```text
ARCADE-1/
├── arcade                          # Executable coeur genere
├── Makefile                        # Build core + plugins + tests
├── README.md                       # README minimal
├── TODO.TXT
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
├── doc/
│   └── STRUCTURE_ACTUELLE.md       # Ce document
├── include/
│   ├── check_main.hpp              # Validation des arguments CLI
│   ├── dll_cmds.hpp                # Loader dynamique (.so)
│   ├── exception.hpp
│   ├── IObserver.hpp
│   ├── Singleton.hpp
│   ├── core/
│   │   ├── core.hpp                # Classe Core
│   │   └── menu.hpp                # Menu principal
│   ├── games/
│   │   ├── arcade_snake.hpp
│   │   ├── IEvents.hpp
│   │   └── IGames.hpp              # Interface plugin jeu
│   └── graphicals/
│       ├── arcade_ncurses.hpp
│       ├── arcade_sdl2.hpp
│       └── IGraphic.hpp            # Interface plugin graphique
├── lib/
│   ├── arcade_ncurses.so           # Plugin graphique ncurses
│   ├── arcade_sdl2.so              # Plugin graphique SDL2
│   └── arcade_snake.so             # Plugin jeu Snake
├── src/
│   ├── main.cpp                    # Point d'entree
│   ├── core/
│   │   ├── Core.cpp                # Boucle principale et orchestration
│   │   └── menu.cpp                # Navigation menu
│   ├── games/
│   │   └── arcade_snake.cpp        # Implementation jeu Snake (etat simple)
│   └── graphicals/
│       ├── arcade_ncurses.cpp      # Backend ncurses
│       └── arcade_sdl2.cpp         # Backend SDL2
└── tests/
    └── tests_main.cpp
```

## 3) Role des composants

- `src/main.cpp`
  - Verifie les arguments (`./lib/*.so`), puis instancie `Core`.
- `Core` (`include/core/core.hpp`, `src/core/Core.cpp`)
  - Charge la librairie graphique initiale.
  - Gere la boucle de rendu et d'evenements.
  - Gere le menu et le passage menu <-> jeu.
  - Permet de changer de backend graphique a chaud.
- `Menu` (`include/core/menu.hpp`, `src/core/menu.cpp`)
  - Liste les jeux/libs graphiques disponibles (actuellement liste codee en dur).
  - Retourne une action de chargement selon les entrees utilisateur.
- Interfaces:
  - `IGraphic` definit le contrat des backends d'affichage.
  - `IGames` definit le contrat des jeux chargeables.
- Plugins graphiques:
  - `arcade_ncurses.so` implemente le rendu terminal.
  - `arcade_sdl2.so` implemente le rendu fenetre SDL2/TTF.
- Plugin jeu:
  - `arcade_snake.so` implemente un squelette de jeu Snake (encore minimal).

## 4) Schema textuel du fonctionnement

```text
[Utilisateur]
    |
    | lance: ./arcade ./lib/arcade_ncurses.so
    v
[main.cpp]
    |
    | verifie chemin + extension .so + existence fichier
    v
[Core]
    |
    | charge dynamiquement IGraphic via DLLoader
    | symbole attendu: createGraphical
    v
[IGraphic concret]
(ncurses OU sdl2)
    |
    | init + boucle evenement/rendu
    v
[Menu]
    |
    | ENTER -> charge un jeu
    | S     -> switch lib graphique
    v
+-------------------------+
| Si jeu selectionne      |
| Core charge IGames via  |
| DLLoader (createGame)   |
+-------------------------+
    |
    v
[IGames concret]
(Snake)
    |
    | handleInput -> update -> draw
    v
[IGraphic concret]
    |
    | display()
    v
[Ecran]
```

## 5) Dependances de build (Makefile)

- Executable coeur:
  - `arcade` compile depuis `src/main.cpp` + `src/core/*.cpp`
  - lien avec `-ldl`
- Plugins:
  - `lib/arcade_ncurses.so` avec `-lncurses`
  - `lib/arcade_sdl2.so` avec `-lSDL2 -lSDL2_ttf`
  - `lib/arcade_snake.so`
- Tests:
  - cible `tests_run` avec Criterion + gcovr

## 6) Design patterns utilises actuellement

### 6.1) Plugin + Factory Method (actif)

- Idee:
  - Le coeur ne connait pas les classes concretes des libs.
  - Il demande une instance via un symbole C exporte (`createGraphical` ou `createGame`).
- Dans ce projet:
  - `DLLoader<T>::getInstance(symbol)` resolve le symbole avec `dlsym` et appelle la fonction de creation.
  - `Core` charge ainsi un backend graphique ou un jeu sans dependance compilee sur l'implementation concrete.
- Effet concret:
  - Ajout d'une nouvelle lib = nouveau `.so` + respect de l'interface.
  - Le core reste stable tant que le contrat `IGraphic`/`IGames` est respecte.

### 6.2) Strategy via interfaces (actif)

- Idee:
  - Le comportement varie selon l'implementation choisie d'une interface commune.
- Dans ce projet:
  - `IGraphic` est la strategie d'affichage.
  - `NcursesAdapter` et `Sdl2Adapter` sont deux strategies interchangeables.
  - `IGames` est la strategie de gameplay chargee dynamiquement (`SnakeGame` actuellement).
- Effet concret:
  - Changement de rendu a chaud depuis le menu (`S`) sans recompiler le core.
  - Le coeur appelle toujours la meme API abstraite (`init`, `handleEvents`, `draw`, `display`).

### 6.3) Adapter (actif)

- Idee:
  - Encapsuler une API externe (ncurses, SDL2) derriere une interface projet unique.
- Dans ce projet:
  - `NcursesAdapter` traduit les appels `IGraphic` vers ncurses.
  - `Sdl2Adapter` traduit les appels `IGraphic` vers SDL2/SDL_ttf.
- Effet concret:
  - Les jeux et le core dessinent avec une API unique, independamment de la techno graphique.

### 6.4) Observer (present mais peu exploite)

- Idee:
  - Un sujet notifie des observateurs lors d'evenements.
- Dans ce projet:
  - `IEvents` expose `Attach`, `Detach`, `Notify`.
  - `IObserver` expose `Update`.
  - `ArcadeEvent` herite de `IEvents` dans `IGraphic.hpp`.
- Etat actuel:
  - Le mecanisme est structurellement present, mais la boucle principale manipule surtout des evenements sous forme de `std::string`.
  - L'Observer n'est pas encore au centre du flux runtime.

### 6.5) Singleton (declare, pas active dans le flux)

- Idee:
  - Garantir une instance unique partagee (ici pour le chargement d'assets via `IAssetLoader`).
- Dans ce projet:
  - Classe `Singleton` avec `GetInstance(...)` et stockage statique.
- Etat actuel:
  - Le pattern est declare dans le header, mais non branche dans le chemin principal d'execution (`main -> Core -> menu/jeux`).

### 6.6) Mini schema texte: pattern -> fichiers -> impact

```text
[Plugin + Factory Method]
  -> include/dll_cmds.hpp:17 (DLLoader), include/dll_cmds.hpp:46 (getInstance)
  -> src/core/Core.cpp:16, src/core/Core.cpp:31, src/core/Core.cpp:54
  -> src/graphicals/arcade_ncurses.cpp:187 (createGraphical)
  -> src/graphicals/arcade_sdl2.cpp:198 (createGraphical)
  -> src/games/arcade_snake.cpp:42 (createGame)
  => Impact: chargement runtime de plugins sans liaison compilee directe.

[Strategy]
  -> include/graphicals/IGraphic.hpp:77
  -> include/games/IGames.hpp:14
  -> include/graphicals/arcade_ncurses.hpp:13, include/graphicals/arcade_sdl2.hpp:17
  -> include/games/arcade_snake.hpp:13
  -> src/core/Core.cpp:25 (switch graphique), src/core/Core.cpp:46 (load jeu)
  => Impact: comportements interchangeables (rendu et jeu) sous meme contrat.

[Adapter]
  -> include/graphicals/arcade_ncurses.hpp:13 + src/graphicals/arcade_ncurses.cpp
  -> include/graphicals/arcade_sdl2.hpp:17 + src/graphicals/arcade_sdl2.cpp
  -> cible adaptee: include/graphicals/IGraphic.hpp:77
  => Impact: ncurses/SDL2 exposes via une API projet unifiee.

[Observer]
  -> include/IObserver.hpp:13
  -> include/games/IEvents.hpp:14 (Attach/Detach/Notify aux lignes 19/23/27)
  -> include/graphicals/IGraphic.hpp:52 (ArcadeEvent : IEvents)
  => Impact actuel: infrastructure presente, exploitation runtime encore limitee.

[Singleton]
  -> include/Singleton.hpp:15 (classe), include/Singleton.hpp:26 (GetInstance)
  => Impact actuel: prepare la centralisation d'assets, non active dans la boucle core.
```

## 7) Etat fonctionnel observe

- Le coeur, la selection de backend graphique et le chargement dynamique sont en place.
- Le menu fonctionne, mais sa liste de bibliotheques est actuellement statique (hardcodee dans `menu.cpp`).
- Le jeu Snake est un squelette (initialisation + affichage titre + sortie), logique de gameplay a completer.
