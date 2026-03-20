/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** tests_main
*/

#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include "check_main.hpp"

const std::string libpath = "lib/libexample.so";

Test(Main_test, check_args) {
    const char *argv[] = {"./arcade", libpath.c_str()};
    int argc = 2;
    cr_assert_eq(check_main::check_args(argc, argv), true);
}

Test(Main_test, fail_args) {
    const char *argv[] = {"./arcade"};
    int argc = 1;
    try {
        check_main::check_args(argc, argv);
        cr_assert_fail("Expected exception for missing args");
    } catch (const std::exception &e) {
        cr_assert_str_eq(e.what(), "Usage: ./arcade <library_path>");
    }
}

Test(Main_test, check_lib_folder)
{
    const char *argv[] = {"./arcade", libpath.c_str()};
    int argc = 2;
    cr_assert_eq(check_main::check_lib_folder(argc, argv), true);
}

Test(Main_test, fail_lib_folder)
{
    const char *argv[] = {"./arcade", "example.so"};
    int argc = 2;
    try {
        check_main::check_lib_folder(argc, argv);
        cr_assert_fail("Expected exception for bad lib folder");
    } catch (const std::exception &e) {
        cr_assert_str_eq(e.what(), "Error: Library path must start with 'lib/'");
    }
}

Test(Main_test, check_file_exist)
{
    struct stat st;
    if (stat("lib", &st) == -1) {
        mkdir("lib", 0755);
    }
    FILE *f = fopen(libpath.c_str(), "w");
    fclose(f);
    bool res = check_main::check_file_exist(libpath);
    remove(libpath.c_str());
    cr_assert_eq(res, true);
}

Test(Main_test, fail_file_exist)
{
    try {
        check_main::check_file_exist("lib/nonexistent.so");
        cr_assert_fail("Expected exception for missing file");
    } catch (const std::exception &e) {
        cr_assert_str_eq(e.what(), "Error: File 'lib/nonexistent.so' does not exist");
    }
}