#pragma once

#include "../Common.h"
#include "../GL.h"

namespace boxer {
	struct Shader {
		U32 _ID;
	public:
		Shader();
		~Shader();

		void Bind();

		void AddShader(const char* src, U32 type);
		void Compile();
	};
}