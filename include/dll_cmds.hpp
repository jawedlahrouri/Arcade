/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** dl_cmds
*/

#ifndef DL_CMDS_HPP_
#define DL_CMDS_HPP_
#include <dlfcn.h>
#include <vector>
#include <memory>
#include "exception.hpp"
#include "graphicals/IGraphic.hpp"

    template <typename T>
    class DLLoader {
        private:
            void* _handle;

        public:

        __attribute__((constructor))
        static void initLibrary() {
            //TODO: any initialization code if needed when the library is loaded
        }

        __attribute__((destructor))
        static void cleanupLibrary() {
            //TODO: any cleanup code if needed when the library is unloaded
        }

        DLLoader(const std::string &path) {
            _handle = ::dlopen(path.c_str(), RTLD_LAZY); 
            if (!_handle) {
                throw Exception(::dlerror());
            }
        }

        ~DLLoader() {
            if (_handle) {
                ::dlclose(_handle);
            }
        }

        T* getInstance(const std::string &symbol) {
            ::dlerror();
            void* raw_sym = ::dlsym(_handle, symbol.c_str());
            const char* dlsym_error = ::dlerror();
            
            if (dlsym_error) {
                throw Exception(dlsym_error);
            }
            typedef T* (*CreatorFunc)();
            CreatorFunc creator = reinterpret_cast<CreatorFunc>(raw_sym);
            T* instance = creator();
            return instance;
        }
    };

#endif /* !DL_CMDS_HPP_ */
