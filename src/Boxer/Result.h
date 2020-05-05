#pragma once

#include "Common.h"

namespace boxer {
	template<class T>
	class Result {
	private:
		// When true, the value is available
		bool _Set;
		T _Value;
	public:
		inline Result(T& value, bool set) : _Value(value), _Set(set) {}
		inline ~Result() {}
		
		inline const T& GetValue() const {
			ASSERT(_Set, "Optional has no value.");
			return _Value;
		}
		
		inline bool IsSet() const {
			return _Set;	
		}
	};
}