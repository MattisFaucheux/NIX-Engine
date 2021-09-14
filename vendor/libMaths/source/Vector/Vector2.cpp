//#include "vector2.h"
//
//#include "core/cmath.h"
//
//namespace libmath
//{
//	vector2& vector2::operator/=(const vector2& other)
//	{
//		// todo optimize to only have check in debug
//		if (other.x == 0.0f || other.y == 0.f)
//		{
//			x = 0.0f;
//			y = 0.0f;
//			return *this;
//		}
//
//
//		x /= other.x;
//		y /= other.y;
//		return *this;
//	}
//
//	float vector2::magnitude() const
//	{
//		return sqrt(x * x + y * y);
//	}
//	
//	radian vector2::extractanglebetween(vector2 lhs, vector2 rhs)
//	{
//		float lhssqmagnitude = lhs.squaremagnitude();
//		if (lhssqmagnitude != 1.f)
//		{
//			lhs /= sqrt(lhssqmagnitude);
//		}
//
//		float rhssqmagnitude = rhs.squaremagnitude();
//		if (rhssqmagnitude != 1.f)
//		{
//			rhs /= sqrt(rhssqmagnitude);
//		}
//
//		return acos(lhs.dot(rhs));
//	}
//}