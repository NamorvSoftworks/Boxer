#pragma once

#include "../Common.h"
#include "../GL.h"

namespace boxer {
	struct Shader {
		GLuint _ID;
	public:
		Shader();
		~Shader();

		void Bind();

		void AddShader(const char* src, GLenum type);
		void Compile();
	};
}