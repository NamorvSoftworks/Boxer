#ifdef PLATFORM_WINDOWS

#include <Boxer/Window.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace boxer {
	// TODO(NeGate): Define the Win32 implementation!
	Window::Window(String title, U32 width, U32 height) : _Title(title), _Width(width), _Height(height) {
		
	}

	Window::~Window() {
		
	}

	void Window::Show() {

	}

	void Window::Update() {

	}

	void Window::Close() {

	}
}

#endif /* PLATFORM_WINDOWS */