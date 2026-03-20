/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** IObserver
*/

#ifndef IOBSERVER_HPP_
#define IOBSERVER_HPP_

#include <string>

class IObserver {
    public:
        virtual ~IObserver(){};
        virtual void Update(const std::string &message_from_subject) = 0;
};

#endif /* !IOBSERVER_HPP_ */
