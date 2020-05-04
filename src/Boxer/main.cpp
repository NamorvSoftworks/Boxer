// I added a `PLATFORM_WINDOWS` to the preprocessor defines
// because i needed it and you cant stop me!
// - NeGate (May 3, 2019)
#include "Common.h"
#include "String.h"
#include "Window.h"
#include <stdio.h>

int main(int argc, char** argv) {
	using boxer::String;

	// Random testing, remove later
	String str("Hello, World!");
	printf("%.*s!\n", static_cast<int>(str.Length()), str.Data());
	
	str = "Bye, World!";
	printf("%.*s!\n", static_cast<int>(str.Length()), static_cast<const char*>(str));
	
	const char* string = str;

	// Window stuff
	boxer::Window wnd("Application", 1600, 900);
	wnd.Show();

	while(1) {
		wnd.Update();
	}
	wnd.Close();
	
	return 0;
}
