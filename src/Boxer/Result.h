#pragma once

#include "Common.h"

namespace boxer {
    template<class T>
    struct Optional {
        // When true, the value is available
        bool _Set;
        T _Value;
        
        inline Optional(T& value) : _Value(value) {}
        inline ~Optional() {}
        
        inline const T& GetValue() {
            ASSERT("Optional has no value.", _Set);
            return _Value;
        }
        
        inline bool IsSet() {
            return _Set;	
        }
    };
}