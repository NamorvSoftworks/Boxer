#include "Shader.h"

namespace boxer {
	Shader::Shader() {
		_ID = glCreateProgram();
	}

	Shader::~Shader() {
		glDeleteProgram(_ID);
	}

	void Shader::Bind() {
		glUseProgram(_ID);
	}

	void Shader::AddShader(const char* src, GLenum type) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);
		glAttachShader(_ID, shader);

		GLint result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (!result) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			char* error = new char[length + 1ui64];
			glGetShaderInfoLog(shader, length, &length, error);

			// TODO(NeGate): We need a logging system
			printf("Shader Error: %s\n", error);

			glDeleteShader(shader);
			return;
		}
	}

	void Shader::Compile() {
		glLinkProgram(_ID);

		GLint result;
		glGetProgramiv(_ID, GL_LINK_STATUS, &result);
		if (!result) {
			GLint length;
			glGetProgramiv(_ID, GL_INFO_LOG_LENGTH, &length);

			// This memory allocation is super insignificant
			char* error = new char[length + 1ui64];
			Defer(delete[] error);

			glGetProgramInfoLog(_ID, length, &length, error);

			// TODO(NeGate): We need a logging system
			printf("Program Error: %s\n", error);
			glDeleteProgram(_ID);
			return;
		}
	}

}