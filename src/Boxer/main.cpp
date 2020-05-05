// I added a `PLATFORM_WINDOWS` to the preprocessor defines
// because i needed it and you cant stop me!
// - NeGate (May 3, 2019)
#include "Common.h"
#include "String.h"
#include "App.h"
#include <stdio.h>

int main(int argc, char** argv) {
	// Window stuff
	boxer::Application app("Application", 1600, 900);
	app.Launch();
	return 0;
}
