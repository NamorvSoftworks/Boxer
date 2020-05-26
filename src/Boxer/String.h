#pragma once

#include "Common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace boxer {
	inline static bool IsWhitespace(char c) {
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}
	
	class String {
	private:
		size_t _Length;
		const char* _Data;
	public:
		// Creates empty string
		inline String() = default;

		// Converts a null-terminated string
		inline String(const char* str) {
			_Length = strlen(str);
			_Data = str;
		}

		inline String(const char* str, size_t len) {
			_Length = len;
			_Data = str;
		}

		inline String(const String& str) {
			_Length = str._Length;
			_Data = str._Data;
		}

		inline ~String() = default;

		inline String SubString(size_t off, size_t len) const {
			ASSERT(off+len <= _Length, "Substring out-of-bounds!");
			return String(&_Data[off], len);
		}
		inline String SubString(size_t off) const {
			ASSERT(off <= _Length, "Substring out-of-bounds!");
			return String(&_Data[off], _Length - off);
		}
		inline String Trim() {
			if (_Length == 0) return String(_Data, 0);

			String res(_Data, _Length);

			const char* strEnd = _Data + (_Length-1);
			while (strEnd != _Data && IsWhitespace(*strEnd)) {
				res._Length--;
				strEnd--;
			}

			return res;
		}
		inline String ConsumeLine() {
			size_t off = IndexOf('\n')+1;

			String res = SubString(0, off);
			_Data += off;
			_Length -= off;

			return res;
		}
		inline String EatWord(size_t start = 0) {
			size_t off = start;
			while (off < _Length) {
				if (IsWhitespace(_Data[off])) break;
				off++;
			}

			return SubString(off);
		}
		inline String EatSpaces(size_t start = 0) {
			size_t off = start;
			while(off < _Length) {
				if (!IsWhitespace(_Data[off])) break;
				off++;
			}

			return SubString(off);
		}

		template<size_t _Size>
		inline void Unicode(wchar_t (&out)[_Size], size_t outLength = _Size) const {
			// TODO(NeGate): This is probably broken.
			ASSERT(outLength >= _Length + 1);

			for (U64 i = 0; i < _Length; i++) {
				out[i] = static_cast<wchar_t>(_Data[i]);
			}
			out[_Length] = '\0';
		}

		// NOTE(NeGate): You have to delete the memory for this!
		// NOTE(NeGate): Use Defer if you don't want to worry about deleting it.
		inline wchar_t* Unicode() const {
			// TODO(NeGate): This is probably broken.
			wchar_t* str = new wchar_t[_Length + 1];
			for(U64 i = 0; i < _Length; i++) {
				str[i] = static_cast<wchar_t>(_Data[i]);
			}
			str[_Length] = '\0';

			return str;
		}

		// NOTE(NeGate): You have to delete the memory for this!
		// NOTE(NeGate): Use Defer if you don't want to worry about deleting it.
		inline char* CStr() const {
			char* cstr = new char[_Length + 1];
			memcpy(cstr, _Data, _Length);
			cstr[_Length] = 0;

			return cstr;
		}

		inline size_t Find(const String& str, size_t off = 0) const {
			for (size_t i = off; i < _Length; i++) {
				if (_Data[i] == str[0] && memcmp(_Data + i, str._Data, str._Length) == 0) {
					return i;
				}
			}

			return -1;
		}

		inline size_t Find(const char* str, size_t off = 0) const {
			size_t strLen = strlen(str);

			for (size_t i = off; i < _Length; i++) {
				if (_Data[i] == str[0] && memcmp(_Data + i, str, strLen) == 0) {
					return i;
				}
			}

			return -1;
		}

		inline size_t IndexOf(char ch, size_t off = 0) const {
			for (size_t i = off; i < _Length; i++) {
				if (_Data[i] == ch) {
					return i;
				}
			}

			return -1;
		}

		inline size_t LastIndexOf(char ch, size_t off = 0) const {
			for (size_t i = _Length-1; i >= off; i--) {
				if (_Data[i] == ch) {
					return i;
				}
			}

			return -1;
		}

		inline bool Contains(const String& str) const {
			return Find(str) != -1;
		}
		inline bool Contains(const char* str) const {
			return Find(str) != -1;
		}
		inline bool Contains(char ch) const {
			return IndexOf(ch) != -1;
		}

		inline bool StartsWith(const String& str) const {
			size_t min = str._Length;
			if (_Length < min) min = _Length;

			return memcmp(_Data, str._Data, min) == 0;
		}
		inline bool StartsWith(const char* str) const {
			size_t min = strlen(str);
			if (_Length < min) min = _Length;

			return memcmp(_Data, str, min) == 0;
		}
		inline bool StartsWith(char ch) const {
			return _Length > 0 && _Data[0] == ch;
		}

		inline bool operator!=(const String& other) const {
			if (_Length == other._Length) return true;

			return memcmp(_Data, other._Data, _Length) != 0;
		}

		inline bool operator==(const char* other) const {
			size_t otherLen = strlen(other);
			if (_Length != otherLen) return false;

			return memcmp(_Data, other, _Length) == 0;
		}

		inline bool operator==(const String& other) const {
			if (_Length != other._Length) return false;

			return memcmp(_Data, other._Data, _Length) == 0;
		}

		inline const char& operator[](size_t i) const {
			return _Data[i];
		}

		inline operator const char*() const {
			return _Data;
		}

		inline size_t Length() const {
			return _Length;
		}

		inline const char* Data() const {
			return _Data;
		}

		inline bool Empty() const {
			return _Length == 0;
		}

		// For iterators
		inline const char& begin() const {
			return _Data[0];
		}
		inline const char& end() const {
			return _Data[_Length - 1];
		}
	};
}