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
        /**
         * @brief Builds an exception with a custom message.
         * @param message Exception message text.
         * @return Does not return a value.
         */
        explicit Exception(const std::string &message) : _message(message) {}
        /**
         * @brief Returns the stored exception message.
         * @return Null-terminated C string message.
         */
        const char* what() const noexcept override {
            return _message.c_str();
        }
};

class WarningException : public Exception {
    public:
        /**
         * @brief Builds a warning-level exception.
         * @param message Warning message text.
         * @return Does not return a value.
         */
        explicit WarningException(const std::string &message) : Exception(message) {}
};

class ErrorException : public Exception {
    public:
        /**
         * @brief Builds an error-level exception.
         * @param message Error message text.
         * @return Does not return a value.
         */
        explicit ErrorException(const std::string &message) : Exception(message) {}
};

class CrashException : public Exception {
    public:
        /**
         * @brief Builds a crash-level exception.
         * @param message Crash message text.
         * @return Does not return a value.
         */
        explicit CrashException(const std::string &message) : Exception(message) {}
};

#endif /* !EXCEPTION_HPP_ */
