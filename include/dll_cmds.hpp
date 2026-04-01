/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** dl_cmds
*/

#ifndef DL_CMDS_HPP_
#define DL_CMDS_HPP_
#include <dlfcn.h>
#include <string>
#include <vector>
#include <memory>
#include "exception.hpp"
#include "graphicals/IGraphic.hpp"

    template <typename T>
    class DLLoader {
        private:
            void* _handle;

        public:

        /**
         * @brief Hook called when the shared library is loaded.
         * @return Does not return a value.
         */
        __attribute__((constructor))
        static void initLibrary() {
            //TODO: any initialization code if needed when the library is loaded
        }

        /**
         * @brief Hook called when the shared library is unloaded.
         * @return Does not return a value.
         */
        __attribute__((destructor))
        static void cleanupLibrary() {
            //TODO: any cleanup code if needed when the library is unloaded
        }

        /**
         * @brief Opens a dynamic library handle from a filesystem path.
         * @param path Path to the shared object file.
         * @return Does not return a value.
         */
        DLLoader(const std::string &path) {
            _handle = ::dlopen(path.c_str(), RTLD_LAZY); 
            if (!_handle) {
                throw Exception(::dlerror());
            }
        }

        /**
         * @brief Closes the dynamic library handle if it is open.
         * @return Does not return a value.
         */
        ~DLLoader() {
            if (_handle) {
                ::dlclose(_handle);
            }
        }

        /**
         * @brief Resolves a factory symbol and returns a typed instance.
         * @param symbol Name of the exported factory symbol.
         * @return Pointer to the created instance.
         */
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

        /**
         * @brief Resolves and calls a type getter symbol from the loaded library.
         * @param symbol Name of the exported type symbol.
         * @return Integer identifying the library kind.
         */
        int getType(const std::string &symbol = "getType") {
            ::dlerror();
            void* raw_sym = ::dlsym(_handle, symbol.c_str());
            const char* dlsym_error = ::dlerror();

            if (dlsym_error) {
                throw Exception(dlsym_error);
            }
            typedef int (*TypeGetterFunc)();
            TypeGetterFunc getter = reinterpret_cast<TypeGetterFunc>(raw_sym);
            return getter();
        }
    };

#endif /* !DL_CMDS_HPP_ */
