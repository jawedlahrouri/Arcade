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
        /**
         * @brief Validates that the provided library path starts with ./lib/.
         * @param argv Command-line arguments array.
         * @return true when the path prefix is valid.
         */
        static bool check_lib_folder(const char **argv);
        /**
         * @brief Checks whether a file exists on disk.
         * @param path File path to validate.
         * @return true when the file exists.
         */
        static bool check_file_exist(const std::string &path);
        /**
         * @brief Validates that the selected file has a .so extension.
         * @param argv Command-line arguments array.
         * @return true when the file extension is .so.
         */
        static bool check_is_so(const char **argv);
        /**
         * @brief Runs all startup argument checks.
         * @param ac Number of command-line arguments.
         * @param av Command-line arguments array.
         * @return true when all checks pass.
         */
        static bool check_args(int ac, const char **av);
};

#endif /* !CHECK_MAIN_HPP_ */
