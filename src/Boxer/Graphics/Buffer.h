#pragma once

#include "../Common.h"

namespace boxer {
	class Buffer {
		U32 _ID;
		U32 _Target;
	public:
		Buffer(U32 target);
		~Buffer();

		void Bind();
		void Unbind();

		void Upload(void* data, size_t size, U32 usage);
	};
}