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

		ASSERT(RegisterClassExW(&wc), "Win32: Failed to register window class.");

		const DWORD style = WS_OVERLAPPEDWINDOW;
		const DWORD exStyle = WS_EX_APPWINDOW;

		wchar_t titleStr[64];
		title.Unicode(titleStr);

		// Get the size of the border.
		RECT borderRect = { 0, 0, 0, 0 };
		AdjustWindowRectEx(&borderRect, style, false, exStyle);

		RECT rect;
		GetClientRect(GetDesktopWindow(), &rect);

		int X = (rect.right / 2) - (_Width / 2);
		int Y = (rect.bottom / 2) - (_Height / 2);
		int Width = _Width;
		int Height = _Height;

		// Border rectangle in this case is negative.
		X += borderRect.left;
		Y += borderRect.top;

		// Grow the window size by the OS border. This makes the client width/height correct.
		Width += borderRect.right - borderRect.left;
		Height += borderRect.bottom - borderRect.top;

		// Create the window.
		win32->hWnd = CreateWindowExW(
			exStyle, CLASS_NAME, titleStr, style,
			X, Y, Width, Height,
			0, 0, win32->hInstance, 0
		);

		ASSERT(win32->hWnd, "Win32: Failed to create a window.");

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
		if (err != GLEW_OK) {
			const char* errorStr = (const char*)glewGetErrorString(err);
			ASSERT(0, errorStr);
		}

		// This looks nasty
		// TODO: Migrate this
		WGLSWAPINTERVALEXT* wglSwapIntervalEXT = (WGLSWAPINTERVALEXT*)wglGetProcAddress("wglSwapIntervalEXT");

		// wglSwapIntervalEXT is the function to enable/disable vsync
		wglSwapIntervalEXT(0);
	}

	void Application::Launch() {
		Win32Handle* win32 = reinterpret_cast<Win32Handle*>(_Handle);

		Timer::Initialize();

		// No you wont run the game at more than 4 billion fps...
		U32 FPS = 0;
		F64 fpsTimer = 0.0;

		// You can change this.
		F64 maxFps = 300.0;
		F64 minFrametime = 0.0;// 1.0 / maxFps;

		F64 lastT, nowT, elapsedT;
		lastT = Timer::Now();

		// Initialize GL
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		while (1) {
			// Event handling
			MSG message;
			if (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0) {
				TranslateMessage(&message);
				DispatchMessage(&message);

				if (message.message == WM_QUIT) break;
			}

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