#ifdef PLATFORM_WINDOWS

#include <Boxer/Window.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace boxer {
	// This can be changed
	const wchar_t CLASS_NAME[] = L"BOXER_ENGINE";

	struct Win32Handle {
		HINSTANCE	hInstance;
		HWND		hWnd;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// TODO(NeGate): Define the Win32 implementation!
	Window::Window(String title, U32 width, U32 height) : _Title(title), _Width(width), _Height(height) {
		Win32Handle* win32 = new Win32Handle();
		_Handle = win32;

		win32->hInstance = GetModuleHandleA(0);
		
		// Register the window class.
		WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.hInstance = win32->hInstance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = NULL;

		if (RegisterClassExW(&wc) == 0) {
			// TODO(NeGate): We need a logging system
			ASSERT(false, "Win32: Failed to register window class.");
		}

		const DWORD style = WS_OVERLAPPEDWINDOW;
		const DWORD exStyle = WS_EX_APPWINDOW;

		RECT rect;
		GetClientRect(GetDesktopWindow(), &rect);
		rect.left = (rect.right / 2) - ((_Width + 16) / 2);
		rect.top = (rect.bottom / 2) - ((_Height + 39) / 2);

		wchar_t* titleStr = title.Unicode();
		Defer(delete[] titleStr);

		// Create the window.
		win32->hWnd = CreateWindowExW(
			exStyle,
			CLASS_NAME,
			titleStr,
			style,
			rect.left,
			rect.top,
			_Width + 16, // Windows is weird, this makes it so that _Width and _Height
			_Height + 39, // are the actually size of the window's viewport.
			0,
			0,
			win32->hInstance,
			0
		);

		if (win32->hWnd == NULL) {
			ASSERT(0, "Win32: Failed to create a window.");
			return;
		}
	}

	Window::~Window() {
		Win32Handle* win32 = reinterpret_cast<Win32Handle*>(_Handle);

		PostQuitMessage(0);
		delete _Handle;
	}

	void Window::Show() {
		Win32Handle* win32 = reinterpret_cast<Win32Handle*>(_Handle);
		ShowWindow(win32->hWnd, SW_SHOW);
		SetFocus(win32->hWnd);
	}

	void Window::Update() {
		// TODO: Setup message queue
		MSG message;
		while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void Window::Close() {

	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

#endif /* PLATFORM_WINDOWS */