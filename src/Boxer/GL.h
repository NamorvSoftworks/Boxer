#pragma once

#include "Common.h"

#include <GL/glew.h>
#include <GL/GL.h>

#ifdef _DEBUG
#define GLCall(x) x; _GLCheckErrors_(__FILE__, __LINE__)
#else
#define GLCall(x) x
#endif

inline static void _GLCheckErrors_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		const char* error = nullptr;
		switch (errorCode) {
		// TODO: Add more errors
		case GL_INVALID_ENUM:					error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:					error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:				error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:					error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:				error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:					error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: error = "UNKNOWN"; break;
		}

		// REPORT ERROR
		// TODO(Yasser): We need a logging system
		printf_s("%s (Code: %d) | %s ( %d )\n", error, errorCode, file, line);
	}
}