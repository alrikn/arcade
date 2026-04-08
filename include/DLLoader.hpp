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
#include "Error.hpp"

template <typename T>
class DLLoader {
    private:
        void* _handle;

        //destroy func pointer given by the lib
        void (*_destroyFunc)(T*);
        T* _instance;

    public:
        DLLoader(const std::string& path) : _handle(nullptr), _destroyFunc(nullptr), _instance(nullptr)
        {
            _handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!_handle)
                throw DLLoadError(dlerror());
        }

        ~DLLoader()
        {
            if (_instance && _destroyFunc) {
                _destroyFunc(_instance);
                _instance = nullptr;
            }
            if (_handle) {
                dlclose(_handle);
                _handle = nullptr;
            }
        }

        void reset()
        {
            if (_instance && _destroyFunc) {
                _destroyFunc(_instance);
                _instance = nullptr;
            }
            _instance = nullptr;
            _destroyFunc = nullptr;
            if (_handle) {
                dlclose(_handle);
                _handle = nullptr;
            }
        }

        void setHandle(const std::string& path)
        {
            reset();
            _handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!_handle)
                throw DLLoadError(dlerror());
        }

        T* getInstance()
        {
            void* sym = dlsym(_handle, "create");
            if (!sym)
                throw DLLoadError(dlerror());

            T* (*create)() = reinterpret_cast<T* (*)()>(sym);

            sym = dlsym(_handle, "destroy");
            if (!sym)
                throw DLLoadError(dlerror());
            _destroyFunc = reinterpret_cast<void (*)(T*)>(sym);
            _instance = create();
            return _instance;
        }
    };
#endif
