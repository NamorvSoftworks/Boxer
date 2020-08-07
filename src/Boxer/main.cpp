// I added a `PLATFORM_WINDOWS` to the preprocessor defines
// because i needed it and you cant stop me!
// - NeGate (May 3, 2020)
#include "Common.h"
#include "String.h"
#include "App.h"
#ifdef PLATFORM_WINDOWS
#include <crtdbg.h> // include to bring _CrtSetDbgFlag into scope
#endif
#include "Math/Math.h"

int main(int argc, char** argv) {
#if PLATFORM_WINDOWS
	//dump memory leaks when main exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Window stuff
	boxer::Application app("Application", 1600, 900);
	app.Launch();
	return 0;
}
