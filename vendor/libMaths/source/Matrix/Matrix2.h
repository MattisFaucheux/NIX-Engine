#pragma once

#include <cstring>
#include <Vector/TVector2.h>
namespace LibMath
{
	
	struct Matrix2
	{
		Matrix2() { Clear(); }
		Matrix2(Matrix2 const& other) = default;
		Matrix2& operator=(const Matrix2& other) = default;
		Matrix2(float diagonalValue);
		Matrix2(float real, float imaginary); // complex number
		Matrix2(float a, float b, float c, float d);
		~Matrix2() = default;

		static Matrix2 Identity() { return Matrix2(1.f); }

		Matrix2& operator*=(Matrix2 const& other) { *this = *this * other; }
		friend Matrix2 operator*(Matrix2 const& lhs, Matrix2 const& rhs);
		friend TVector2<float> operator*(TVector2<float> const& lhs, Matrix2 const& rhs);

		void Clear() { std::memset(&raw, 0, sizeof(raw)); }

		float const* Data() const { return raw; }

		float raw[4];
	};
}