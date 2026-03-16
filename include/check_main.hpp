/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** check_main
*/

#ifndef CHECK_MAIN_HPP_
#define CHECK_MAIN_HPP_

#include <string>

class check_main {
    public:
        static bool check_lib_folder(int argc, const char **argv);
        static bool check_file_exist(const std::string &path);
        static bool check_args(int ac, const char **av);
};

#endif /* !CHECK_MAIN_HPP_ */
