#ifdef PLATFORM_WINDOWS

#include "Boxer/App.h"
#include "Boxer/Timer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

// WGL Extensions
typedef int* (__stdcall WGLSWAPINTERVALEXT)(int value);

namespace boxer {
	// TODO(NeGate): Allow for multiple windows, this would require different
	// class identifiers for different windows.
	const wchar_t CLASS_NAME[] = L"BOXER_ENGINE";

	struct Win32Handle {
		// NOTE(NeGate): Does this need to be here?
		// Its an application constant. GetModuleHandleW(0);
		HINSTANCE	hInstance;
		HWND		hWnd;
		HDC			hDC;
		HGLRC		hGL;
	};

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// NOTE(NeGate): These are some platform-specific globals.
	LARGE_INTEGER m_Start;
	F64 m_Frequency;

	// Time in seconds
	F64 Timer::Now() {
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		U64 cycles = current.QuadPart - m_Start.QuadPart;

		return (cycles * m_Frequency);
	}

	// Debugging purposes
	U64 Timer::NowInCycles() {
		// NOTE(NeGate): This isnt a Win32 specific function, its an x64 instruction.
		return __rdtsc();
	}

	static PIXELFORMATDESCRIPTOR GetPixelFormat() {
		PIXELFORMATDESCRIPTOR result = {};
		result.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		result.nVersion = 1;
		result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		result.iPixelType = PFD_TYPE_RGBA;
		result.cColorBits = 32;
		result.cDepthBits = 24;
		result.cStencilBits = 8;
		result.cAuxBuffers = 0;
		result.iLayerType = PFD_MAIN_PLANE;
		return result;
	}

	Application::Application(String title, U32 width, U32 height) : _Title(title), _Width(width), _Height(height) {
		Win32Handle* win32 = new Win32Handle();
		_Handle = win32;

		win32->hInstance = GetModuleHandleW(0);
		
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

		wchar_t titleStr[64];
		title.Unicode(titleStr);

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

		SetWindowLongPtrW(win32->hWnd, GWLP_USERDATA, (LONG_PTR)this);

		ShowWindow(win32->hWnd, SW_SHOW);
		SetFocus(win32->hWnd);

		PIXELFORMATDESCRIPTOR pfd = GetPixelFormat();

		win32->hDC = GetDC(win32->hWnd);

		int format = ChoosePixelFormat(win32->hDC, &pfd);
		SetPixelFormat(win32->hDC, format, &pfd);

		win32->hGL = wglCreateContext(win32->hDC);
		wglMakeCurrent(win32->hDC, win32->hGL);

		GLenum err = glewInit();
		if (GLEW_OK != err) {
			/* Problem: glewInit failed, something is seriously wrong. */
			const char* errorStr = (const char*)glewGetErrorString(err);
			ASSERT(0, errorStr);
		}

		// This looks nasty
		// TODO: Migrate this
		WGLSWAPINTERVALEXT* wglSwapIntervalEXT = (WGLSWAPINTERVALEXT*)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(0);
	}

	void Application::Launch() {
		Win32Handle* win32 = reinterpret_cast<Win32Handle*>(_Handle);

		// Initialize Timer
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_Frequency = 1.0 / F64(frequency.QuadPart);
		QueryPerformanceCounter(&m_Start);
		
		// Used to eep
		F64 lastT, nowT, elapsedT;
		lastT = Timer::Now();

		// No you wont run the game at more than 2 billion fps...
		U32 FPS = 0;
		F64 fpsTimer = 0.0;

		// You can change this.
		F64 maxFps = 300.0;
		F64 minFrametime = 1.0 / maxFps;

		// Initialize GL
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		while(1) {
			// Event handling
			MSG message;
			if (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			if (message.message == WM_QUIT) break;

			// FPS limiter.
			do {
				nowT = Timer::Now();
				elapsedT = nowT - lastT;
			} while (elapsedT < minFrametime);

			// TODO: Update the game
			{

			}

			// Render the game
			{
				GLCall(glViewport(0, 0, _Width, _Height));
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

				SwapBuffers(win32->hDC);
				FPS++;
			}

			// FPS Counter
			fpsTimer += elapsedT;

			if (fpsTimer > 1.0) {
				wchar_t titleStr[64];
				_Title.Unicode(titleStr);

				wchar_t temp[64];
				swprintf_s(temp, L"%.*s - FPS %d\n", _Title.Length(), titleStr, FPS);
				SetWindowText(win32->hWnd, temp);

				fpsTimer -= 1.0;
				FPS = 0;
			}

			// Advance to the next frame
			lastT = nowT;
		}

		Close();
	}

	void Application::Close() {
		Win32Handle* win32 = reinterpret_cast<Win32Handle*>(_Handle);

		wglDeleteContext(win32->hGL);

		UnregisterClassW(CLASS_NAME, GetModuleHandleW(nullptr));
		PostQuitMessage(0);
		delete _Handle;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		void* ptr = (void*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		Application* app = reinterpret_cast<Application*>(ptr);

		switch (message) {
		case WM_DESTROY:
		case WM_CLOSE: {
			// TODO(NeGate): Implement weird message system
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

#endif /* PLATFORM_WINDOWS */