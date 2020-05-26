#pragma once

#include "Boxer/Common.h"

namespace boxer {
	struct Timer {
		// Time in seconds
		static F64 Now();
		
		// Debugging purposes
		static U64 NowInCycles();
	};
}