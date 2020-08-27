#ifdef PLATFORM_WINDOWS

#include <Boxer/App.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <Boxer/File.h>
#include <Boxer/Timer.h>

#include <Boxer/Graphics/Buffer.h>
#include <Boxer/Graphics/Shader.h>

// Some useful WGL constants
// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

// Some useful WGL functions
typedef BOOL(WINAPI PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef HGLRC(WINAPI PFNWGLCREATECONTEXTATTRIBSARB)(HDC hDC, HGLRC hshareContext, const int* attribList);
typedef BOOL(WINAPI PFNWGLCHOOSEPIXELFORMATARB)(HDC hdc, const int* piAttribIList,
	const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

PFNWGLCREATECONTEXTATTRIBSARB* wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARB* wglChoosePixelFormatARB;
PFNWGLSWAPINTERVALEXTPROC* wglSwapIntervalEXT;

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

	static void InitGLExt() {
		// Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
		// We use a dummy window because you can only set the pixel format for a window once. For the
		// real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
		// that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
		// have a context.
		WNDCLASSA window_class = { 0 };
		window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		window_class.lpfnWndProc = DefWindowProcA;
		window_class.hInstance = GetModuleHandle(0);
		window_class.lpszClassName = "DummyWindowGL";

		if (!RegisterClassA(&window_class)) {
			ASSERT(0, "Failed to register dummy OpenGL window.");
		}

		HWND dummy_window = CreateWindowExA(
			0,
			window_class.lpszClassName,
			"Dummy OpenGL Window",
			0,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			window_class.hInstance,
			0);

		if (!dummy_window) {
			ASSERT(0, "Failed to create dummy OpenGL window.");
		}

		HDC dummy_dc = GetDC(dummy_window);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
		if (!pixel_format) {
			ASSERT(0, "Failed to find a suitable pixel format.");
		}

		if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
			ASSERT(0, "Failed to set the pixel format.");
		}

		HGLRC dummy_context = wglCreateContext(dummy_dc);
		if (!dummy_context) {
			ASSERT(0, "Failed to create a dummy OpenGL rendering context.");
		}

		if (!wglMakeCurrent(dummy_dc, dummy_context)) {
			ASSERT(0, "Failed to activate dummy OpenGL rendering context.");
		}

		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARB*)wglGetProcAddress(
			"wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARB*)wglGetProcAddress(
			"wglChoosePixelFormatARB");
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC*)wglGetProcAddress(
			"wglSwapIntervalEXT");

		wglMakeCurrent(dummy_dc, 0);
		wglDeleteContext(dummy_context);
		ReleaseDC(dummy_window, dummy_dc);
		DestroyWindow(dummy_window);
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

		wchar_t titleStr[64];
		title.Unicode(titleStr);
		
		// Create the window.
		win32->hWnd = CreateWindowExW(
			exStyle, CLASS_NAME, titleStr, style,
			X, Y, Width, Height,
			0, 0, win32->hInstance, 0
		);
		
		if (win32->hWnd == NULL) {
			ASSERT(0, "Win32: Failed to create a window.");
			return;
		}

		SetWindowLongPtrW(win32->hWnd, GWLP_USERDATA, (LONG_PTR)this);

		ShowWindow(win32->hWnd, SW_SHOW);
		SetFocus(win32->hWnd);

		// This is how you initialize modern OpenGL because Windows...
		{
			win32->hDC = GetDC(win32->hWnd);

			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR) };
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.cAuxBuffers = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int format = ChoosePixelFormat(win32->hDC, &pfd);
			SetPixelFormat(win32->hDC, format, &pfd);

			InitGLExt();

			int pixel_format_attribs[] = {
				WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
				WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
				WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB,         32,
				WGL_DEPTH_BITS_ARB,         24,
				WGL_STENCIL_BITS_ARB,       8,
				0
			};

			int pixel_format;
			UINT num_formats;
			wglChoosePixelFormatARB(win32->hDC, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
			if (!num_formats) {
				ASSERT(0, "Failed to set the OpenGL pixel format.");
			}

			//PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(win32->hDC, pixel_format, sizeof(pfd), &pfd);
			if (!SetPixelFormat(win32->hDC, pixel_format, &pfd)) {
				ASSERT(0, "Failed to set OpenGL pixel format.");
			}

			int gl33_attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0,
			};

			win32->hGL = wglCreateContextAttribsARB(win32->hDC, 0, gl33_attribs);
			if (!win32->hGL) {
				ASSERT(0, "Failed to create OpenGL context.");
			}

			if (!wglMakeCurrent(win32->hDC, win32->hGL)) {
				ASSERT(0, "Failed to activate OpenGL context.");
			}
		}

		GLenum err = glewInit();
		if (err != GLEW_OK) {
			const char* errorStr = (const char*)glewGetErrorString(err);
			ASSERT(0, "GLEW ERROR: %s", errorStr);
		}
		
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
		F64 minFrametime = 1.0 / maxFps;

		F64 lastT, nowT, elapsedT;
		lastT = Timer::Now();

		// Initialize GL
		GLCall(glClearColor(0.0f, 0.0f, 0.2f, 1.0f));

		{
			GLCall(glDisable(GL_STENCIL_TEST));
			GLCall(glDisable(GL_DEPTH_TEST));
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE));

			Shader shader;
			shader.AddShader(FileBlock("assets/Standard.vert").Data, GL_VERTEX_SHADER);
			shader.AddShader(FileBlock("assets/Standard.frag").Data, GL_FRAGMENT_SHADER);
			shader.Compile();

			GLuint VAO;
			GLCall(glGenVertexArrays(1, &VAO));
			GLCall(glBindVertexArray(VAO));

			GLCall(glEnableVertexAttribArray(0));
			
			float data[] = { 
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0,
				1.0, 1.0, 0.0
			};
			Buffer vbo(GL_ARRAY_BUFFER);
			vbo.Bind();
			vbo.Upload(data, sizeof(data), GL_STATIC_DRAW);

			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));
			GLCall(glBindVertexArray(0));
			
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

					glBindVertexArray(VAO);
					vbo.Bind();

					shader.Bind();

					GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

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