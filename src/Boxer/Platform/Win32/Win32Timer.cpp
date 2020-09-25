#ifdef PLATFORM_WINDOWS

#include "Boxer/Timer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

namespace boxer {

	// NOTE(NeGate): These are some platform-specific globals.
	static LARGE_INTEGER s_Start;
	static F64 s_Frequency;

	void Timer::Initialize() {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		s_Frequency = 1.0 / F64(frequency.QuadPart);
		QueryPerformanceCounter(&s_Start);
	}

	F64 Timer::Now() {
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		U64 ticks = current.QuadPart - s_Start.QuadPart;

		return (ticks * s_Frequency);
	}

	U64 Timer::NowInCycles() {
		// NOTE(NeGate): This isnt a Win32 specific function, its an x64 instruction.
		return __rdtsc();
	}

}

#endif /* PLATFORM_WINDOWS */
