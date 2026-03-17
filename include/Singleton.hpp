/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** Singleton
*/

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <string>
#include <memory> // Pour std::unique_ptr
#include "./graphicals/Igraphic.hpp"

class Singleton {
    private:
        static std::unique_ptr<Singleton> singleton_;
        std::string value_;
        std::unique_ptr<IAssetLoader> assetLoader_;

    protected:
        Singleton(const std::string value, std::unique_ptr<IAssetLoader> loader)
            : value_(value), assetLoader_(std::move(loader)) {}

    public:
        static Singleton *GetInstance(const std::string& value, std::unique_ptr<IAssetLoader> loader) {
            if (!singleton_) {
                singleton_ = std::unique_ptr<Singleton>(new Singleton(value, std::move(loader)));
            }
            return singleton_.get();
        }

        void loadtextures(const std::string &path) {
            if (assetLoader_) {
                assetLoader_->loadTexture(path);
            }
        }

        void loadaudio(const std::string &path) {
            if (assetLoader_) {
                assetLoader_->loadAudio(path);
            }
        }

        std::string value() const {
            return value_;
        }
};

#endif /* !SINGLETON_HPP_ */
