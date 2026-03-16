/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** main
*/

#include <iostream>
#include "exception.hpp"
#include <string>
#include <exception>
#include <unistd.h>
#include "check_main.hpp"

bool check_main::check_lib_folder(int argc, const char **argv)
{
    (void)argc;
    std::string lib_path = argv[1];
    if (lib_path.find("lib/") != 0) {
        throw Exception("Error: Library path must start with 'lib/'");
    }
    return true;
}

bool check_main::check_file_exist(const std::string &path)
{
    if (access(path.c_str(), F_OK) != 0) {
        throw Exception("Error: File '" + path + "' does not exist");
    }
    return true;
}

bool check_main::check_args(int ac, const char **av)
{
    if (ac != 2) {
        throw Exception("Usage: " + std::string(av[0]) + " <library_path>");
    }
    check_lib_folder(ac, av);
#ifndef UNIT_TEST
    check_file_exist(std::string(av[1]));
#endif
    return true;
}

#ifndef UNIT_TEST
int main(int argc, char **argv)
{
    try {
        check_main::check_args(argc, const_cast<const char **>(argv));
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    std::cout << "Hello World!" << std::endl;
    return 0;
}
#endif