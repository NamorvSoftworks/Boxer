#pragma once

#include "Boxer/Common.h"

namespace boxer {
	struct Timer {
		static void Initialize();

		// Time in seconds
		static F64 Now();
		
		// Debugging purposes
		static U64 NowInCycles();
	};
}