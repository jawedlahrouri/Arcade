# 🕹️ Arcade — Modular Dynamic Gaming Platform in C++20

[![Academic Year](https://img.shields.io/badge/Epitech-Tek2-green?style=for-the-badge&logo=epitech&logoColor=white)](https://www.epitech.eu/)
[![Module](https://img.shields.io/badge/Module-G--OOP--400-informational?style=for-the-badge)](#)
[![Language](https://img.shields.io/badge/Language-C%2B%2B20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](#)

## 📘 Description
**Arcade** est une plateforme de jeux d'arcade modulaire conçue en **C++20**. Son architecture repose sur le découplage total entre le cœur applicatif, les moteurs de jeux (Snake, Pacman, etc.) et les bibliothèques graphiques (SFML, SDL2, Ncurses) chargées dynamiquement à chaud sous forme de bibliothèques partagées (`.so`) via `dlopen`/`dlsym`.

## ⚙️ Architecture & Design Patterns
- **Abstraction Graphique (`IGraphicLib`)** : Interface commune permettant de commuter instantanément de bibliothèque graphique en pleine partie sans interrompre le jeu.
- **Abstraction de Jeu (`IGameLib`)** : Logique de jeu indépendante de toute bibliothèque graphique.
- **Gestionnaire Dynamique (`DLLoader`)** : Encapsulation RAII des handles de bibliothèques dynamiques.

## 🔨 Compilation & Utilisation
```bash
make
./arcade ./lib/arcade_sfml.so
```
