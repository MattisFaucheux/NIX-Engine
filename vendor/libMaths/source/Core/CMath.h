#pragma once

namespace LibMath
{
	const float MY_FLT_EPSILON = 1.192092896e-07F; // smallest such that 1.0+FLT_EPSILON != 1.0

	inline float Absolute(const float data)
	{
		return (data >= 0.0f) ? data : data * -1;
	};

	float ceil(float value);
	float floor(float value);
	float fmod(float value, float modulo);
	float ldexpf(float value, int baseShift);
	float Powf(float x, float y);
	float sqrt(float value);

	template <class T>
	T Clamp(const T target, const T min, const T max)
	{
		return target <= min ? min : target >= max ? max : target;
	}

	constexpr float NaN = static_cast<float>(1e+300 * 1e+300) * 0.f;

	inline bool ApproxFloat(const float& a, const float& b, const float tolerance = MY_FLT_EPSILON)
	{
		return Absolute(a - b) < tolerance;
	}
}
