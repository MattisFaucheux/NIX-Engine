#include "Quaternion.h"

#include "Matrix/Matrix4.h"

using namespace LibMath;

Quaternion::Quaternion() : X(0.f), Y(0.f), Z(0.f), W(1.f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w)
{
}

LibMath::Quaternion::Quaternion(LibMath::Degree angle, LibMath::Vector3& axis)
{
	LibMath::Degree newAngle = angle;

	newAngle.data = angle.data * 0.5f;

	float fSin = sin(newAngle);

	X = fSin * axis.x;
	Y = fSin * axis.y;
	Z = fSin * axis.z;
	W = cos(newAngle);
	
}

LibMath::Quaternion::Quaternion(LibMath::Radian pitch, LibMath::Radian yaw, LibMath::Radian roll)
{
	LibMath::Radian halfPitch = pitch * .5f;
	LibMath::Radian halfYaw = yaw * .5f;
	LibMath::Radian halfRoll = roll * .5f;
	LibMath::Vector3 cosine = LibMath::Vector3(cos(halfPitch), cos(halfYaw), cos(halfRoll));
	LibMath::Vector3 sinus = LibMath::Vector3(sin(halfPitch), sin(halfYaw), sin(halfRoll));

	W = cosine.x * cosine.y * cosine.z + sinus.x * sinus.y * sinus.z;
	X = sinus.x * cosine.y * cosine.z - cosine.x * sinus.y * sinus.z;
	Y = cosine.x * sinus.y * cosine.z + sinus.x * cosine.y * sinus.z;
	Z = cosine.x * cosine.y * sinus.z - sinus.x * sinus.y * cosine.z;
}

LibMath::Quaternion::Quaternion(LibMath::Radian pitch, LibMath::Radian yaw, LibMath::Radian roll, LibMath::RotationSequence order) : X(0.f), Y(0.f), Z(0.f), W(1.f)
{
	LibMath::Quaternion p(sin(pitch * .5f), .0f, .0f, cos(pitch * .5f));
	LibMath::Quaternion y(.0f, sin(yaw * .5f), .0f, cos(yaw * .5f));
	LibMath::Quaternion r(.0f, .0f, sin(roll * .5f), cos(roll * .5f));

	if ((char)order & (char)LibMath::RotationOrder::Xfirst)
	{
		(*this) *= p;
	}
	else if ((char)order & (char)LibMath::RotationOrder::Yfirst)
	{
		(*this) *= y;
	}
	else
	{
		(*this) *= r;
	}


	if ((char)order & (char)LibMath::RotationOrder::Xsecond)
	{
		(*this) *= p;
	}
	else if ((char)order & (char)LibMath::RotationOrder::Ysecond)
	{
		(*this) *= y;
	}
	else
	{
		(*this) *= r;
	}


	if ((char)order & (char)LibMath::RotationOrder::Xlast)
	{
		(*this) *= p;
	}
	else if ((char)order & (char)LibMath::RotationOrder::Ylast)
	{
		(*this) *= y;
	}
	else
	{
		(*this) *= r;
	}
}

LibMath::Quaternion::Quaternion(const LibMath::Matrix4& mat4)
{
	(*this) = mat4;
}


bool Quaternion::operator==(const Quaternion& other) const
{
	if (X == other.X && Y == other.Y && Z == other.Z && W == other.W)
	{
		return true;
	}

	return false;
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	if (*this == other)
	{
		return false;
	}

	return true;
}

Quaternion Quaternion::operator=(const Quaternion& other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	W = other.W;
	return *this;
}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
}

Quaternion LibMath::Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion tempQuaternion;

	tempQuaternion.X = (Y * other.Z) - (Z * other.Y) + (W * other.X) + (other.W * X);
	tempQuaternion.Y = (Z * other.X) - (X * other.Z) + (W * other.Y) + (other.W * Y);
	tempQuaternion.Z = (X * other.Y) - (Y * other.X) + (W * other.Z) + (other.W * Z);
	tempQuaternion.W = (W * other.W) - (X * other.X) - (Y * other.Y) - (Z * other.Z);

	/*tempQuaternion.X = (other.W * X) + (other.X * W) + (other.Y * Z) - (other.Z * Y);
	tempQuaternion.Y = (other.W * Y) + (other.Y * W) + (other.Z * X) - (other.X * Z);
	tempQuaternion.Z = (other.W * Z) + (other.Z * W) + (other.X * Y) - (other.Y * X);
	tempQuaternion.W = (other.W * W) - (other.X * X) - (other.Y * Y) - (other.Z * Z);*/

    return tempQuaternion;
}

Quaternion Quaternion::operator*(float f) const
{
	return Quaternion(f * X, f * Y, f * Z, f * W);
}

Quaternion& LibMath::Quaternion::operator*=(float f)
{
	X *= f;
	Y *= f;
	Z *= f;
	W *= f;

	return *this;
}

Quaternion& LibMath::Quaternion::operator*=(const Quaternion& other) 
{
	*this = *this * other;
	return *this;
}

Quaternion& LibMath::Quaternion::operator=(const Matrix4& mat4)
{

	const float diag = mat4.raw[0] + mat4.raw[5] + mat4.raw[10] + 1;

	if (diag > 0.0f)
	{
		const float scale = sqrtf(diag) * 2.0f; // get scale from diagonal

		X = (mat4.raw[6] - mat4.raw[9]) / scale;
		Y = (mat4.raw[8] - mat4.raw[2]) / scale;
		Z = (mat4.raw[1] - mat4.raw[4]) / scale;
		W = 0.25f * scale;
	}
	else
	{
		if (mat4.raw[0] > mat4.raw[5] && mat4.raw[0] > mat4.raw[10])
		{
			// 1st element of diag is greatest value
			 // find scale according to 1st element, and double it
			const float scale = sqrtf(1.0f + mat4.raw[0] - mat4.raw[5] - mat4.raw[10]) * 2.0f;

			// TODO: speed this up
			X = 0.25f * scale;
			Y = (mat4.raw[4] + mat4.raw[1]) / scale;
			Z = (mat4.raw[2] + mat4.raw[8]) / scale;
			W = (mat4.raw[6] - mat4.raw[9]) / scale;
		}
		else if (mat4.raw[5] > mat4.raw[10])
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const float scale = sqrtf(1.0f + mat4.raw[5] - mat4.raw[0] - mat4.raw[10]) * 2.0f;

			// TODO: speed this up
			X = (mat4.raw[4] + mat4.raw[1]) / scale;
			Y = 0.25f * scale;
			Z = (mat4.raw[9] + mat4.raw[6]) / scale;
			W = (mat4.raw[8] - mat4.raw[2]) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			 // find scale according to 3rd element, and double it
			const float scale = sqrtf(1.0f + mat4.raw[10] - mat4.raw[0] - mat4.raw[5]) * 2.0f;

			// TODO: speed this up
			X = (mat4.raw[8] + mat4.raw[2]) / scale;
			Y = (mat4.raw[9] + mat4.raw[6]) / scale;
			Z = 0.25f * scale;
			W = (mat4.raw[1] - mat4.raw[4]) / scale;
		}
	}
#pragma warning(push, 0) // Removing all vulkan enum warnings because we're compiling in w4
	this->Normalize();
	return *this;
#pragma pop

}

Vector3 Quaternion::operator*(const Vector3& other) const
{
	float vMult = 2.0f * (X * other.x + Y * other.y + Z * other.z);
	float crossMult = 2.0f * W;
	float pMult = crossMult * W - 1.0f;

	return Vector3(	pMult * other.x + vMult * X + crossMult * (Y * other.z - Z * other.y),
					pMult * other.y + vMult * Y + crossMult * (Z * other.x - X * other.z),
					pMult * other.z + vMult * Z + crossMult * (X * other.y - Y * other.x));
}

Vector3 LibMath::Quaternion::Rotate(const Vector3& other) const
{
	Quaternion newQuaternion(*this);
	float sqNorm = X * X + Y * Y + Z * Z + W * W;
	if (sqNorm != 1.f)
	{
		float norm = sqrt(sqNorm);
		newQuaternion.X /= norm;
		newQuaternion.Y /= norm;
		newQuaternion.Z /= norm;
		newQuaternion.W /= norm;
	}

	float vMult = 2.0f * (newQuaternion.X * other.x + newQuaternion.Y * other.y + newQuaternion.Z * other.z);
	float crossMult = 2.0f * newQuaternion.W;
	float pMult = crossMult * newQuaternion.W - 1.0f;

	return Vector3(pMult * other.x + vMult * newQuaternion.X + crossMult * (newQuaternion.Y * other.z - newQuaternion.Z * other.y),
		pMult * other.y + vMult * newQuaternion.Y + crossMult * (newQuaternion.Z * other.x - newQuaternion.X * other.z),
		pMult * other.z + vMult * newQuaternion.Z + crossMult * (newQuaternion.X * other.y - newQuaternion.Y * other.x));
}

Quaternion LibMath::Quaternion::Multiply(const Quaternion& other) const
{
	return *this * other;
}

Quaternion& LibMath::Quaternion::MultiplyEqual(const Quaternion& other)
{
	return *this *= other;
}

Quaternion LibMath::Quaternion::MultiplyF(float f) const
{
	return *this * f;
}

Quaternion& LibMath::Quaternion::MultiplyEqualF(float f)
{
	return *this *= f;
}

float LibMath::Quaternion::DotProduct(const Quaternion& other) const
{
	return (X * other.X) + (Y * other.Y) + (Z * other.Z) + (W * other.W);
}

void LibMath::Quaternion::Normalize()
{
	float f = Norm();

	if (f != 0.f)
	{
		X /= f;
		Y /= f;
		Z /= f;
		W /= f;
	}
}

Quaternion LibMath::Quaternion::GetNormalize() const
{
	Quaternion newQuaternion(*this);

	float f = newQuaternion.Norm();

	if (f != 0.f)
	{
		newQuaternion.X /= f;
		newQuaternion.Y /= f;
		newQuaternion.Z /= f;
		newQuaternion.W /= f;
	}
	else
	{
		newQuaternion.W = 1.f;
	}

	return newQuaternion;
}

void LibMath::Quaternion::Conjugate(const Quaternion& other)
{
	X = -other.X;
	Y = -other.Y;
	Z = -other.Z;
	W = other.W;
}

void LibMath::Quaternion::Inverse()
{
	X = -X;
	Y = -Y;
	Z = -Z;
}

void Quaternion::Negate()
{
	X = -X;
	Y = -Y;
	Z = -Z;
	W = -W;
}

Quaternion LibMath::Quaternion::GetInverse() const
{
	Quaternion newQuaternion(*this);
	newQuaternion.X = -newQuaternion.X;
	newQuaternion.Y = -newQuaternion.Y;
	newQuaternion.Z = -newQuaternion.Z;

	return newQuaternion;
}

float LibMath::Quaternion::Norm()
{
	return sqrt(X * X + Y * Y + Z * Z + W * W);
}

LibMath::Radian LibMath::Quaternion::GetPitch() const
{
	float y = 2.f * ((Y * Z) + (W * X));
	float x = (W * W) - (X * X) - (Y * Y) + (Z * Z);

	if (x == 0.f && y == 0.f)
	{
		return atan2(X, W);
	}

	return atan2(y, x);
}

LibMath::Radian LibMath::Quaternion::GetYaw() const
{
	float ratio = -2.f * (X * Z - W * Y);
	ratio = ratio < -1.f ? -1.f : ratio > 1.f ? 1.f : ratio;

	return asin(ratio);
}

LibMath::Radian LibMath::Quaternion::GetRoll() const
{
	float y = 2.f * ((X * Y) + (W * Z));
	float x = (W * W) + (X * X) - (Y * Y) - (Z * Z);

	return atan2(y, x);
}

LibMath::Matrix4 LibMath::Quaternion::ToMatrix4() const
{
	Quaternion quat = GetNormalize();

	float y2 = quat.Y * quat.Y;	float wz = quat.W * quat.Z;	float x2 = quat.X * quat.X;
	float z2 = quat.Z * quat.Z;	float xz = quat.X * quat.Z;	float yz = quat.Y * quat.Z;
	float xy = quat.X * quat.Y;	float wy = quat.W * quat.Y;	float wx = quat.W * quat.X;

	Matrix4 mat;

	mat[0][0] = 1.0f - (2 * y2) - (2 * z2);		mat[0][1] = (2 * xy) - (2 * wz);			mat[0][2] = (2 * xz) + (2 * wy);			 mat[0][3] = 0;
	mat[1][0] = (2 * xy) + (2 * wz);			mat[1][1] = 1.0f - (2 * x2) - (2 * z2);		mat[1][2] = (2 * yz) - (2 * wx);			 mat[1][3] = 0;
	mat[2][0] = (2 * xz) - (2 * wy);			mat[2][1] = (2 * yz) + (2 * wx);			mat[2][2] = 1.0f - (2 * x2) - (2 * y2);		 mat[2][3] = 0;
	mat[3][0] = 0;								mat[3][1] = 0;								mat[3][2] = 0;								 mat[3][3] = 1;

	return mat;
}

LibMath::Vector3 LibMath::Quaternion::EulerAngles() const
{
	if (this == new Quaternion{ 0.5f, 0.5f, -0.5f, 0.5f }) return { 90.0f, 90.0f, 0.0f };
	if (this == new Quaternion{ 0.5f, 0.5f, 0.5f, -0.5f }) return { -90.0f, -90.0f, 0.0f };

	Vector3 angles;

	float sinr_cosp = 2 * (W * X + Y * Z);
	float cosr_cosp = 1 - 2 * (X * X + Y * Y);
	angles.x = atan2(sinr_cosp, cosr_cosp).deg();

	float sinp = 2 * (W * Y - Z * X);
	if (Absolute(sinp) >= 1)
		angles.y = Rad(copysign(PI / 2, sinp)).deg();
	else
		angles.y = asin(sinp).deg();

	float siny_cosp = 2 * (W * Z + X * Y);
	float cosy_cosp = 1 - 2 * (Y * Y + Z * Z);
	angles.z = atan2(siny_cosp, cosy_cosp).deg();

	return angles;
}