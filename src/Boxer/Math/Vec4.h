#pragma once

#include <Boxer/Common.h>
#include "MathFunctions.h"

namespace boxer {

	__declspec(align(16)) union Vec4 {
		struct { F32 x, y, z, w; };
		__m128 Internal;

		INLINE Vec4() : Internal(_mm_setzero_ps()) {}
		INLINE Vec4(F32 scalar) : Internal(_mm_set1_ps(scalar)) {}
		INLINE Vec4(F32 _x, F32 _y, F32 _z, F32 _w) : x(_x), y(_y), z(_z), w(_w) {}
		INLINE Vec4(__m128 xmm) : Internal(xmm) {}

		INLINE F32 Dot(const Vec4 other) const {
			return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
		}
		INLINE Vec4 Normalize() const {
			F32 len = Sqrt(Dot(*this));

			// NOTE: This is completely unchecked
			return (*this / len);
		}
		INLINE F32 Magnitude() const {
			return Sqrt(Dot(*this));
		}
		INLINE F32 MagnitudeSquared() const {
			return Dot(*this);
		}
		INLINE bool operator==(const Vec4& other) {
			int mask = _mm_movemask_ps(_mm_cmp_ps(Internal, other.Internal, _CMP_EQ_OQ));
			return (mask & 0b1111) == 0;
		}
		INLINE bool operator!=(const Vec4& other) {
			int mask = _mm_movemask_ps(_mm_cmp_ps(Internal, other.Internal, _CMP_EQ_OQ));
			return (mask & 0b1111) != 0;
		}
		INLINE Vec4& Add(const Vec4& other) {
			Internal = _mm_add_ps(Internal, other.Internal);
			return *this;
		}
		INLINE Vec4& Subtract(const Vec4& other) {
			Internal = _mm_sub_ps(Internal, other.Internal);
			return *this;
		}
		INLINE Vec4& Multiply(const Vec4& other) {
			Internal = _mm_mul_ps(Internal, other.Internal);
			return *this;
		}
		INLINE Vec4& Divide(const Vec4& other) {
			Internal = _mm_div_ps(Internal, other.Internal);
			return *this;
		}
		INLINE Vec4 operator+(const Vec4& other) const {
			return Vec4(_mm_add_ps(Internal, other.Internal));
		}
		INLINE Vec4 operator-(const Vec4& other) const {
			return Vec4(_mm_sub_ps(Internal, other.Internal));
		}
		INLINE Vec4 operator*(const Vec4& other) const {
			return Vec4(_mm_mul_ps(Internal, other.Internal));
		}
		INLINE Vec4 operator/(const Vec4& other) const {
			return Vec4(_mm_div_ps(Internal, other.Internal));
		}
		INLINE F32 operator[](size_t index) const {
			return Internal.m128_f32[index];
		}
		INLINE Vec4& operator+=(const Vec4& other) {
			return Add(other);
		}
		INLINE Vec4& operator-=(const Vec4& other) {
			return Subtract(other);
		}
		INLINE Vec4& operator*=(const Vec4& other) {
			return Multiply(other);
		}
		INLINE Vec4& operator/=(const Vec4& other) {
			return Divide(other);
		}
	};
}