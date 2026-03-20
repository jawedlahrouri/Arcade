/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** IEvents
*/

#ifndef IEVENTS_HPP_
#define IEVENTS_HPP_
#include "./IObserver.hpp"
#include <vector>
#include <algorithm>

class IEvents {
    public:
        IEvents() = default;
        virtual ~IEvents() = default;

        void Attach(IObserver *observer) {
            observers.push_back(observer);
        }

        void Detach(IObserver *observer) {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void Notify(const std::string &message) {
            for (auto *observer : observers) {
                observer->Update(message);
            }
        }

    private:
        std::vector<IObserver *> observers;
};

#endif /* !IEVENTS_HPP_ */
