#include "Buffer.h"
#include "../GL.h"

namespace boxer {
	Buffer::Buffer(U32 target) {
		_Target = target;
		GLCall(glGenBuffers(1, &_ID));
	}

	Buffer::~Buffer() {
		GLCall(glDeleteBuffers(1, &_ID));
	}

	void Buffer::Bind() {
		GLCall(glBindBuffer(_Target, _ID));
	}

	void Buffer::Unbind() {
		GLCall(glBindBuffer(_Target, 0));
	}

	void Buffer::Upload(void* data, size_t size, U32 usage) {
		GLCall(glBufferData(_Target, size, data, usage));
	}
}