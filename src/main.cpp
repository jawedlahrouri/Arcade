/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** main
*/

#include <iostream>

static bool check_args(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <library_path>" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (!check_args(argc, argv)) {
        return 84;
    }
    std::cout << "Hello World!" << std::endl;
    return 0;
}