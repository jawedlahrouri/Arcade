# ARCADE-1

Tests group

Tests have been done with the group:

`joseph.birck@epitech.eu.jpg`

## Features

- Core executable loading dynamic `.so` plugins at runtime.
- Interchangeable graphic backends (ncurses and SDL2).
- Unified input system with `Arcade::InputCommand`.
- Menu that scans `./lib` to list available games and graphic libraries.

## Requirements

- g++ (C++20)
- make
- ncurses
- SDL2
- SDL2_ttf

## Build

```bash
make
```

Useful commands:

```bash
make clean
make fclean
make re
```

## Run

```bash
./arcade ./lib/arcade_ncurses.so
./arcade ./lib/arcade_sdl2.so
```

## Controls

- Arrow keys: menu navigation / in-game movement
- ENTER: launch selected game
- S: switch graphic backend at runtime
- ESC: back to menu
- Q: quit

## Project structure

- `include/`: interfaces and core headers
- `src/`: implementations
- `lib/`: generated plugins
- `asset/`: project resources
- `doc/`: project documentation

## Internal docs

- `doc/structure.current.md`: up-to-date architecture and project scan
- `doc/structure.old2.md`: previous snapshot
