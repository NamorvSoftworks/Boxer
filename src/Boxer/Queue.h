#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace boxer {
    template<class T>
    struct Queue {
        size_t _Size;
        size_t _Capacity;

        T* _Elements;
        
        inline Queue() {
            _Size = 0;
            _Capacity = 0;
            _Elements = nullptr;
        }

        inline Queue(const size_t capacity) {
            _Size = 0;
            _Capacity = capacity;

            if (capacity == 0) { _Elements = 0; return; }
            _Elements = (T*)malloc(_Capacity * sizeof(T));
        }

        inline ~Queue() {
            Delete();
        }

        inline void Delete() {
            if (_Elements) delete _Elements;
            _Elements = nullptr;
        }

        inline void Clear() {
            _Size = 0;
        }

        inline void Resize(const size_t size) {
            if (_Size != size) {
                _Size = size;
                _Capacity = size;

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
            if (size >= _Capacity) {
                _Capacity = size * 2;

                void* old = _Elements;
                _Elements = (T*)realloc((void*)_Elements, _Capacity * sizeof(T));
                if (_Elements == nullptr) free(old); // If the realloc fails, free the old pointer
            }
        }

        inline size_t Enqueue(const T& elem) {
            ReserveCapacity(_Size);

            memcpy(
                (void*)(&_Elements[_Size]),
                (const void*)(&elem),
                sizeof(T)
            );
            _Size++;
            return _Size - 1;
        }
        inline void Dequeue() {
            // TODO: Test this, its probably broken
            _Size--;
            memmove(
                (void*)(_Elements),
                (const void*)(&_Elements[1]),
                sizeof(T) * _Size
            );
        }
        
        inline size_t IndexOf(T elem) {
            for (size_t i = 0; i < _Size; i++) {
                if (_Elements[i] == elem) {
                    return i;
                }
            }

            return -1;
        }

        inline Queue<T>& operator+=(T& t) {
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