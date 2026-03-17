/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** DLLoader
*/

#ifndef INCLUDED_DLLOADER_HPP
    #define INCLUDED_DLLOADER_HPP

#include <dlfcn.h>
#include <stdexcept>

template <typename T>
class DLLoader {
    private:
        void* _handle;

        //destroy func pointer given by the lib
        void (*_destroyFunc)(T*);
        T* _instance;

    public:
        DLLoader(const std::string& path)
        {
            _handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!_handle)
                throw std::runtime_error(dlerror());
        }

        ~DLLoader()
        {
            _destroyFunc(_instance);
            if (_handle)
                dlclose(_handle);
        }

        T* getInstance()
        {
            void* sym = dlsym(_handle, "create");
            if (!sym)
                throw std::runtime_error(dlerror());

            T* (*create)() = reinterpret_cast<T* (*)()>(sym);

            sym = dlsym(_handle, "destroy");
            if (!sym)
                throw std::runtime_error(dlerror());
            _destroyFunc = reinterpret_cast<void (*)(T*)>(sym);
            _instance = create();
            return _instance;
        }
    };
#endif
