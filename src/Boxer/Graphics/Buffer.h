#pragma once

#include "../Common.h"
#include "../GL.h"

namespace boxer {
	class Buffer {
		GLuint _ID;
		GLenum _Target;
	public:
		Buffer(GLenum target);
		~Buffer();

		void Bind();
		void Unbind();

		void Upload(void* data, size_t size, GLenum usage);
	};
}