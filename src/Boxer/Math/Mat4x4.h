//Copyright (c) 2020 Namorv Softworks
#pragma once

#include <Boxer/Common.h>
#include "Vec4.h"
#include "MathFunctions.h"

namespace boxer {

	// NOTE(NeGate): SIMD usually requires proper natural alignment to run
	__declspec(align(16)) union Mat4x4 {
		F32 Elements[4][4];
		__m128 Columns[4];

		INLINE Mat4x4(F32 dig = 1.0) {
			Columns[0] = _mm_setr_ps(dig , 0.0f, 0.0f, 0.0f);
			Columns[1] = _mm_setr_ps(0.0f, dig , 0.0f, 0.0f);
			Columns[2] = _mm_setr_ps(0.0f, 0.0f, dig , 0.0f);
			Columns[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, dig );
		}

		INLINE Mat4x4& operator+(F32 scalar) {
			const __m128 scalar128 = _mm_set1_ps(scalar);
			Columns[0] = _mm_add_ps(Columns[0], scalar128);
			Columns[1] = _mm_add_ps(Columns[1], scalar128);
			Columns[2] = _mm_add_ps(Columns[2], scalar128);
			Columns[3] = _mm_add_ps(Columns[3], scalar128);
			return *this;
		}
		INLINE Mat4x4& operator-(F32 scalar) {
			const __m128 scalar128 = _mm_set1_ps(scalar);
			Columns[0] = _mm_sub_ps(Columns[0], scalar128);
			Columns[1] = _mm_sub_ps(Columns[1], scalar128);
			Columns[2] = _mm_sub_ps(Columns[2], scalar128);
			Columns[3] = _mm_sub_ps(Columns[3], scalar128);
			return *this;
		}
		INLINE Mat4x4 operator*(F32 scalar) {
			const __m128 scalar128 = _mm_set1_ps(scalar);
			Columns[0] = _mm_mul_ps(Columns[0], scalar128);
			Columns[1] = _mm_mul_ps(Columns[1], scalar128);
			Columns[2] = _mm_mul_ps(Columns[2], scalar128);
			Columns[3] = _mm_mul_ps(Columns[3], scalar128);
			return *this;
		}
		INLINE Mat4x4 operator/(F32 scalar) {
			const __m128 scalar128 = _mm_set1_ps(scalar);
			Columns[0] = _mm_div_ps(Columns[0], scalar128);
			Columns[1] = _mm_div_ps(Columns[1], scalar128);
			Columns[2] = _mm_div_ps(Columns[2], scalar128);
			Columns[3] = _mm_div_ps(Columns[3], scalar128);
			return *this;
		}
		// Linear combination
		INLINE static __m128 LaneMultiply(const __m128& a, const Mat4x4& b) {
			__m128 result;
			result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), b.Columns[0]);
			result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), b.Columns[1]));
			result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), b.Columns[2]));
			result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), b.Columns[3]));
			return result;
		}
		INLINE Mat4x4& Multiply(const Mat4x4& other) {
			Columns[0] = LaneMultiply(Columns[0], other);
			Columns[1] = LaneMultiply(Columns[1], other);
			Columns[2] = LaneMultiply(Columns[2], other);
			Columns[3] = LaneMultiply(Columns[3], other);
			return *this;
		}

		INLINE static Mat4x4& Translate(const Vec4& vc) {
			Mat4x4 result;
			result.Elements[3][0] = vc.x;
			result.Elements[3][1] = vc.y;
			result.Elements[3][2] = vc.z;
			return result;
		}
		INLINE static Mat4x4& Scale(const Vec4& vc) {
			Mat4x4 result;
			result.Columns[0] = _mm_setr_ps(vc.x, 0.0f, 0.0f, 0.0f);
			result.Columns[1] = _mm_setr_ps(0.0f, vc.y, 0.0f, 0.0f);
			result.Columns[2] = _mm_setr_ps(0.0f, 0.0f, vc.z, 0.0f);
			result.Columns[3] = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
			return result;
		}
		INLINE static Mat4x4& Ortho(F32 left, F32 right, F32 bottom, F32 top, F32 near, F32 far) {
			Mat4x4 result;
			result.Elements[0][0] = 2.0f / (right - left);
			result.Elements[1][1] = 2.0f / (top - bottom);
			result.Elements[2][2] = 2.0f / (near - far);
			result.Elements[3][3] = 1.0f;

			result.Elements[3][0] = (left + right) / (left - right);
			result.Elements[3][1] = (bottom + top) / (bottom - top);
			result.Elements[3][2] = (far + near) / (near - far);
			return result;
		}
		INLINE static Mat4x4& Perspective(F32 width, F32 height, F32 fov, F32 near, F32 far) {
			F32 aspect_ratio = width / height;
			F32 cotan = 1.0f / Tan(ToRadians(fov));

			Mat4x4 result;
			result.Elements[0][0] = cotan / aspect_ratio;
			result.Elements[1][1] = cotan;
			result.Elements[2][3] = -1.0f;
			result.Elements[2][2] = (near + far) / (near - far);
			result.Elements[3][2] = (2.0f * near * far) / (near - far);
			result.Elements[3][3] = 0.0f;
			return result;
		}
	};

}
