#pragma once

#include <Boxer/Common.h>

namespace boxer {

	constexpr F32 NATURAL_LOG = 2.71828182846f;
	constexpr F32 PI = 3.14159265359f;
	constexpr F32 PI_OVER_180 = 0.01745329251994329547f;

	INLINE F32 ToRadians(F32 degrees) { return degrees * PI_OVER_180; }
	INLINE F32 ToDegrees(F32 radians) { return radians / PI_OVER_180; }
	INLINE F32 Sqrt(F32 x) { return _mm_cvtss_f32(_mm_sqrt_ps(_mm_set1_ps(x))); }
	INLINE F32 Cos(F32 x) { return _mm_cvtss_f32(_mm_cos_ps(_mm_set1_ps(x))); }
	INLINE F32 Sin(F32 x) { return _mm_cvtss_f32(_mm_sin_ps(_mm_set1_ps(x))); }
	INLINE F32 Tan(F32 x) { return _mm_cvtss_f32(_mm_tan_ps(_mm_set1_ps(x))); }
	INLINE F32 Atan(F32 x) { return _mm_cvtss_f32(_mm_atan_ps(_mm_set1_ps(x))); }
	INLINE F32 Atan2(F32 y, F32 x) { return _mm_cvtss_f32(_mm_atan2_ps(_mm_set1_ps(y), _mm_set1_ps(x))); }
	INLINE F32 Floor(F32 x) { return _mm_cvtss_f32(_mm_round_ps(_mm_set1_ps(x), _MM_FROUND_FLOOR)); }

}