//Copyright (c) 2020 Namorv Softworks
#pragma once
// TODO implement the following:
// • Matrix class
// • Vec2 class
// • Vec4 class
// Stuff everything into Math.cpp

#include <array>
#include <initializer_list>

namespace boxer {
	// todo: make a custom matrix class	
	using matrix = std::initializer_list<std::array<std::array<F32, 4>, 4>>;

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

		// arithmetic operators

		Quat operator+(const Quat& rhs)const;
		//Quat operator+(const F32 rhs)const;
		Quat operator-(const Quat& rhs)const;
		// cross product
		Quat operator*(const Quat& rhs)const;
		Quat& operator=(const Quat& rhs);


		// comparison operators

		bool operator==(const Quat& rhs)const;
		bool operator!=(const Quat& rhs)const;





		// member functions

		// dot product
		Quat dot(const Quat& rhs)const;
		// cross product
		Quat cross(const Quat& rhs)const;		
		
		// matrix conversion
		matrix to_matrix()const;

		[[nodiscard]] F32 normalize()const;
	};

	// vector 3
	class Vec3 
		: public Quat {
	public:
		Vec3(F32 x0, F32 x1, F32 x2);

		// use x, y, z for immediate access
		// use this for array access:
		F32 operator[](size_t index)const;
	};

	// vector 2
	//class Vec2 {};

}