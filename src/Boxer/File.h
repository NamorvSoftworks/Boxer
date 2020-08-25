#pragma once

#include <Boxer/Common.h>
#include <Boxer/String.h>

namespace boxer {
	class FileBlock {
	private:
		void* _Handle;
	public:
		String Path;
		String Data;

		FileBlock(String path);
		~FileBlock();
	};
}