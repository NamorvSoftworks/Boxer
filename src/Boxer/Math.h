//Copyright (c) 2020 Namorv Softworks
#pragma once
// TODO implement the following:
// • Matrix class
// • Vec2 class
// • Vec4 class
// Stuff everything into Math.cpp

namespace boxer {

	// quaternion
	class Quat {
	public:
		// m_ is omitted because its public
		F32 w;
		F32 x;
		F32 y;
		F32 z;

		Quat();
		Quat(F32 a, F32 b, F32 c, F32 d);
		~Quat();

		// operators

		Quat operator+(const Quat& rhs)const noexcept {
			return { w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z };
		}
		Quat operator+(const F32 rhs)noexcept {
			return { w + rhs, x, y, z };
		}
		Quat operator-(const Quat& rhs)const noexcept {
			return { w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z };
		}
		// cross product
		Quat operator*(const Quat& rhs)const noexcept {
			return cross(rhs);
		}
		Quat&
			operator=(const Quat& rhs)noexcept {
			w = rhs.w;
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}

		// member functions

		// dot product
		Quat dot(const Quat& rhs)const noexcept {
			return { w * rhs.w, x * rhs.x, y * rhs.y, z * rhs.z };
		}
		// cross product
		Quat cross(const Quat& rhs)const noexcept {
			return {
			(w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
			(w * rhs.w) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
			(w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
			(w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w)
			};
		}
	
		bool operator==(const Quat& rhs) {
			return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
		}
		bool operator!=(const Quat& rhs) {
			return !operator==(rhs);
		}

		bool operator==(const Quat& rhs)const noexcept {
			return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
		}
		bool operator!=(const Quat& rhs)const noexcept {
			return !(*this == rhs);
		}
		[[nodiscard]] F32 normalize()const;
	};

	// vector 3
	class Vec3 
		: public Quat {
	public:
		Vec3(F32 x0, F32 x1, F32 x2);

		// use x(), y(), z() for compile-time access
		// use this for runtime:
		F32 operator[](size_t index)const;
	};

	// vector 2
	//class Vec2 {};

}