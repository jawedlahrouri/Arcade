/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** exception
*/

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <exception>
#include <string>

class Exception : public std::exception {
    private:
        std::string _message;
    public:
        explicit Exception(const std::string &message) : _message(message) {}
        const char* what() const noexcept override {
            return _message.c_str();
        }
};

class WarningException : public Exception {
    public:
        explicit WarningException(const std::string &message) : Exception(message) {}
};

class ErrorException : public Exception {
    public:
        explicit ErrorException(const std::string &message) : Exception(message) {}
};

class CrashException : public Exception {
    public:
        explicit CrashException(const std::string &message) : Exception(message) {}
};

#endif /* !EXCEPTION_HPP_ */
