/*
** EPITECH PROJECT, 2026
** ARCADE-1
** File description:
** dl_cmds
*/

#ifndef DL_CMDS_HPP_
#define DL_CMDS_HPP_
#include <dlfcn.h>

namespace dl {
    class DlCmds {
        public:
            void* dlopen(const char *file, int mode);
            void* dlsym(void *handle, const char *symbol);
            int dlclose(void *handle);
            const char* dlerror(void);
    };
}

#endif /* !DL_CMDS_HPP_ */
