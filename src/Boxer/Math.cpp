//Copyright (c) 2020 Namorv Softworks
#include "Common.h"
#include "Math.h"
#include <cmath> // get the sqrtf intrinsic from here

namespace boxer {

	//Quat definitions

	Quat::Quat() : w(0.f), x(0.f), y(0.f), z(0.f) {

	}

	Quat::Quat(F32 a, F32 b, F32 c, F32 d)
		: w(a), x(b), y(c), z(d) {

	}

	Quat::~Quat() {
	
	}

	Quat Quat::operator+(const Quat& rhs)const {
		return { w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z };
	}

	/*Quat Quat::operator+(const F32 rhs)const {
		return { w + rhs, x, y, z };
	}*/

	Quat Quat::operator-(const Quat& rhs)const {
		return { w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z };
	}

	bool Quat::operator==(const Quat& rhs)const {
		return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool Quat::operator!=(const Quat& rhs)const {
		return !(*this == rhs);
	}






	//cross product
	Quat Quat::operator*(const Quat& rhs)const {
		return cross(rhs);
	}

	Quat& Quat::operator=(const Quat& rhs) {
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}





	// dot product
	Quat Quat::dot(const Quat& rhs)const {
		return { w * rhs.w, x * rhs.x, y * rhs.y, z * rhs.z };
	}

	// cross product
	Quat Quat::cross(const Quat& rhs)const {
		return {
		(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
		(w * rhs.w) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
		(w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
		(w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w)
		};
	}


	// matrix conversion
	matrix Quat::to_matrix()const {
		using std::sqrt;
		using std::fabs;
		const F32
			normal = normalize(),
			r = w * normal,
			i = x * normal,
			j = y * normal,
			k = z * normal;
		return {
			{1.f - 2.f * sqrt(fabs(j * j + k * k)), 2.f * sqrt(fabs(i * k - k * r)),       2.f * sqrt(fabs(i * k + j * r)),       0.f},
			{2.f * sqrt(fabs(r * j + k * r)),       1.f - 2.f * sqrt(fabs(i * i + k * k)), 2.f * sqrt(fabs(j * k - i * r)),       0.f},
			{2.f * sqrt(fabs(i * k - j * r)),       2.f * sqrt(fabs(j * k + i * r)),       1.f - 2.f * sqrt(fabs(i * i + j * j)), 0.f},
			{0.f,                          0.f,                          0.f,                          1.f}
		};
	}

	F32	Quat::normalize()const {
		using std::sqrtf;
		ASSERT(w || x || y || z, "division by zero");
		return 1.f / sqrtf(w * w + x * x + y * y + z * z);
	}


	// Vec3 definitions

	Vec3::Vec3(F32 x0, F32 x1, F32 x2) 
		: Quat(0.f, x0, x1, x2) {

	}

	F32 Vec3::operator[](size_t index)const {
		switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default: //this case should never be hit
#if _DEBUG
			ASSERT(0, "out of range");
#elif _MSC_VER
			__assume(0);
#endif
		}
	}

}