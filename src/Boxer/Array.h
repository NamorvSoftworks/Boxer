#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace boxer {
    template<class T>
    struct Array {
        size_t _Size;
        size_t _Capacity;

        T* _Elements;
        
        inline Array() {
            _Size = 0;
            _Capacity = 0;
            _Elements = nullptr;
        }

        inline Array(const size_t capacity) {
            _Size = 0;
            _Capacity = capacity;

            if (capacity == 0) { _Elements = 0; return; }
            _Elements = (T*)malloc(_Capacity * sizeof(T));
        }

        inline ~Array() {
            Delete();
        }

        inline void Delete() {
            if (_Elements) delete _Elements;
            _Elements = nullptr;
        }

        inline void Clear() {
            _Size = 0;
        }

        inline void ShrinkToFit() {
            if (_Size != _Capacity) {
                void* old = _Elements;
                _Elements = (T*)realloc((void*)_Elements, _Size * sizeof(T));
                if(_Elements == nullptr) free(old); // If the realloc fails, free the old pointer
                
                _Capacity = _Size;
            }
        }
        inline void Resize(const size_t size) {
            if (_Size != s) {
                _Size = size;
                _Capacity = s;

                if (_Elements == nullptr) {
                    _Elements = (T*)malloc(_Capacity * sizeof(T));
                    return;
                } else {
                    void* old = _Elements;
                    _Elements = (T*)realloc((void*)_Elements, _Capacity * sizeof(T));
                    if (_Elements == nullptr) free(old); // If the realloc fails, free the old pointer
                }
            }
        }
        inline void ReserveCapacity(size_t size) {
            if (s >= _Capacity) {
                _Capacity = s + 16;

                void* old = _Elements;
                _Elements = (T*)realloc((void*)_Elements, _Capacity * sizeof(T));
                if (_Elements == nullptr) free(old); // If the realloc fails, free the old pointer
            }
        }

        inline size_t Add(const T& elem) {
            ReserveCapacity(_Size);

            memcpy(
                (void*)(&_Elements[_Size]),
                (const void*)(&elem),
                sizeof(T)
            );
            _Size++;
            return _Size - 1;
        }
        inline T& AddEmpty() {
            // Just reserves the space without calling any constructors.
            ReserveCapacity(_Size);
            _Size++;
            return _Elements[_Size - 1];
        }
        inline void Remove(size_t index) {
            if (_Size > 1) {
                memcpy(
                    (void*)(&_Elements[index]),
                    (const void*)(&_Elements[_Size - 1]),
                    sizeof(T)
                );
            }
            _Size--;
        }

        inline void Push(T elem) {
            Add(elem);
        }
        inline T& Pop() {
            _Size--;
            return _Elements[_Size];
        }
        inline size_t IndexOf(T elem) {
            for (size_t i = 0; i < _Size; i++) {
                if (_Elements[i] == elem) {
                    return i;
                }
            }

            return -1;
        }

        inline Array<T>& operator+=(T& t) {
            Add(t);
            return *this;
        }

        template<size_t I>
        inline T& At() const {
            return _Elements[I];
        }
        inline T& operator[](size_t i) const {
            return _Elements[i];
        }

        inline size_t Size() const {
            return _Size;
        }

        // For iterators
        inline T& begin() const {
            return _Elements[0];
        }
        inline T& end() const {
            return _Elements[_Size - 1];
        }
    };
}