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
#include <csignal>
#include <cstring>
#include <unistd.h>
#include "check_main.hpp"
#include "./core/core.hpp"

namespace {
    const char *gLibPath = nullptr;
    std::size_t gLibPathLen = 0;

    void handleSigill(int)
    {
        const char msgPrefix[] = "CrashException: Incompatible library detected";
        const char msgMid[] = " while loading: ";
        const char msgSuffix[] = "\nReason: plugin does not match current interfaces (IGraphic/IGames) or exported entry (getType/entryPoint).\nRecompile the plugin against current project headers.\n";

        ::write(STDERR_FILENO, msgPrefix, sizeof(msgPrefix) - 1);
        ::write(STDERR_FILENO, msgMid, sizeof(msgMid) - 1);
        if (gLibPath != nullptr && gLibPathLen > 0) {
            ::write(STDERR_FILENO, gLibPath, gLibPathLen);
        } else {
            const char unknown[] = "<unknown>";
            ::write(STDERR_FILENO, unknown, sizeof(unknown) - 1);
        }
        ::write(STDERR_FILENO, msgSuffix, sizeof(msgSuffix) - 1);
        _exit(84);
    }
}


/**
 * @brief Validates that the library path starts with ./lib/.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return true when the path prefix is valid.
 */
bool check_main::check_lib_folder(int argc, const char **argv)
{
    (void)argc;
    std::string lib_path = argv[1];
    if (lib_path.find("./lib/") != 0) {
        throw Exception("Error: Library path must start with './lib/'");
    }
    return true;
}

/**
 * @brief Checks whether the provided file path exists.
 * @param path Path to test on disk.
 * @return true when the file exists.
 */
bool check_main::check_file_exist(const std::string &path)
{
    if (access(path.c_str(), F_OK) != 0) {
        throw Exception("Error: File '" + path + "' does not exist");
    }
    return true;
}

/**
 * @brief Validates that the selected library has a .so extension.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return true when the extension is .so.
 */
bool check_main::check_is_so(int argc, const char **argv)
{
    (void)argc;
    std::string lib_path = argv[1];
    if (!lib_path.ends_with(".so")) {
        throw Exception("Error: File '" + lib_path + "' is not a .so file");
    }
    return true;
}

/**
 * @brief Runs all command-line validations required to start the program.
 * @param ac Number of command-line arguments.
 * @param av Array of command-line arguments.
 * @return true when every validation passes.
 */
bool check_main::check_args(int ac, const char **av)
{
    if (ac != 2) {
        throw Exception("Usage: " + std::string(av[0]) + " <library_path>");
    }
    check_lib_folder(ac, av);
    check_is_so(ac, av);
#ifndef UNIT_TEST
    check_file_exist(std::string(av[1]));
#endif
    return true;
}

#ifndef UNIT_TEST
/**
 * @brief Program entry point that validates arguments and starts the core loop.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 on success, 84 on argument or startup error.
 */
int main(int argc, char **argv)
{
    if (argc > 1 && argv[1] != nullptr) {
        gLibPath = argv[1];
        gLibPathLen = std::strlen(argv[1]);
    }
    std::signal(SIGILL, handleSigill);

    try {
        check_main::check_args(argc, const_cast<const char **>(argv));
        Core core = Core(argv[1]);
        core.run();
    } catch (const WarningException &e) {
        std::cerr << "Warning: " << e.what() << std::endl;
        return 0;
    } catch (const ErrorException &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    } catch (const CrashException &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    } catch (const Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
#endif