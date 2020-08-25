#pragma once

#include <Boxer/Common.h>
#include <cmath>

namespace boxer {

	constexpr F32 NATURAL_LOG = 2.71828182846f;
	constexpr F32 PI = 3.14159265359f;
	constexpr F32 PI_OVER_180 = 0.01745329251994329547f;

	INLINE F32 ToRadians(F32 degrees) { return degrees * PI_OVER_180; }
	INLINE F32 ToDegrees(F32 radians) { return radians / PI_OVER_180; }
	INLINE F32 Sqrt(F32 x) { return sqrt(x); }
	INLINE F32 Cos(F32 x) { return cos(x); }
	INLINE F32 Sin(F32 x) { return sin(x); }
	INLINE F32 Tan(F32 x) { return tan(x); }
	INLINE F32 Atan(F32 x) { return atan(x); }
	INLINE F32 Atan2(F32 y, F32 x) { return atan2(y, x); }
	INLINE F32 Floor(F32 x) { return floor(x); }
	INLINE F32 Ceil(F32 x) { return ceil(x); }
	INLINE F32 Round(F32 x) { return round(x); }

}