 # ARCADE-1 Architecture Schema

 Ce document présente un schéma simplifié et compatible Mermaid v8.8.0 décrivant les composants et les flux principaux.

 ## 1. Vue globale (simplifiée)

```mermaid
flowchart LR
  User --> Graphic[Graphic plugin]
  Graphic --> Core[Core]
  Core --> Menu[Menu]
  Core --> Game[Game]
  Core --> Graphic
```

 ## 2. Rôles principaux

 - `main.cpp`: vérifie les arguments et instancie `Core` avec la bibliothèque graphique de départ.
 - `Core`: boucle principale, chargement dynamique des plugins, routage des commandes vers `Menu` ou `Game` et ordonnancement des appels de rendu.
 - `Menu`: découverte des plugins, gestion de la sélection, construction d'une `DisplayData` pour le rendu.
 - `IGraphic` et ses implémentations: lecture des entrées natives, conversion en `InputCommand`, dessin des objets et présentation à l'écran.
 - `IGames` et ses implémentations: logique du jeu, traitement des commandes, exposition des objets à dessiner.
 - `arcade_types.hpp`: contrat de types partagés (évite le couplage direct entre jeux et interface graphique).

 ## 3. Cycle d'une frame (simplifié)

```mermaid
flowchart TD
Tick[Core loop tick] --> Poll[Graphic pollInputs]
Poll --> Cmds[vector of InputCommand]
Cmds --> Decide{Global command}

Decide -->|Yes| HandleState[Core handles state transition]
Decide -->|No| InMenu{In menu}

InMenu -->|Yes| MenuHandle[Menu handleInput]
MenuHandle --> MenuDisplay[Menu getDisplayables]
MenuDisplay --> DrawMenu[Graphic draw DisplayData]

InMenu -->|No| GameHandle[Game handleInput]
GameHandle --> GameUpdate[Game update]
GameUpdate --> GameDisplay[Game getDisplayables]
GameDisplay --> DrawGame[Graphic draw SpriteObjects]

HandleState --> Present[Graphic display]
DrawMenu --> Present
DrawGame --> Present
```

 ## 4. Direction des données (entrée → logique → rendu)

```mermaid
flowchart LR
Graphic[Graphic plugin] -->|InputCommands| Core[Core]
Core -->|GlobalCmds QUIT SWITCH| CoreState[State handler]
CoreState --> Menu[Menu]
CoreState --> Game[Game]
Core -->|MenuCommands| Menu
Core -->|GameCommands| Game
Menu -->|DisplayData| Core
Game -->|SpriteObjects| Core
Core -->|DrawCalls| Graphic
Graphic -->|Present| Screen[Screen]
```

 ## 5. Chargement dynamique (séquence simplifiée)

```mermaid
sequenceDiagram
participant Main
participant Core
participant Loader
participant GfxSo
participant GameSo

Main->>Core: start(initialGraphic)
Core->>Loader: open(graphic)
Loader->>GfxSo: resolve(createGraphical)
GfxSo-->>Core: IGraphic instance

Note over Core: boucle d'exécution

Core->>Loader: open(game)
Loader->>GameSo: resolve(createGame)
GameSo-->>Core: IGames instance

Core->>Loader: reload(graphic)
Loader->>GfxSo: resolve(createGraphical)
GfxSo-->>Core: new IGraphic
```

 ## 6. Résumé

 - `Core` est le médiateur central du runtime.
 - Les plugins sont indépendants et ne se connaissent pas directement.
 - Les types partagés sont centralisés dans `arcade_types.hpp` pour éviter le couplage strict entre jeux et interface graphique.

