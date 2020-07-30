#pragma once

#include <Boxer/Common.h>
#include <Boxer/String.h>

namespace boxer {
	class FileBlock {
	private:
		String _Path;
		void* _Handle;
		String _Data;
	public:
		FileBlock(String path);
		~FileBlock();

		// I dont like getters...
		inline const String& GetData() const { return _Data; }
		inline const String& GetPath() const { return _Path; }
	};
}