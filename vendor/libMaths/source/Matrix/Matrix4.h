#pragma once

#include <cstring>

#include "Core/Angle.h"
#include "Vector/Vector.h"
#include "Quaternion/Quaternion.h"

namespace LibMath
{
	struct Vector3;

	struct const_col
	{
		const_col(float const* ptr) : m_ptr(ptr) {}
		float const* m_ptr;

		float operator[](int idx) const { return m_ptr[idx * 4]; }
	};

	struct col
	{
		col(float* ptr) : m_ptr(ptr) {}
		float* m_ptr;

		float& operator[](int idx) { return m_ptr[idx * 4]; }
	};

	struct row
	{
		row(float* ptr) : m_ptr(ptr) {}
		float* m_ptr;

		float& operator[](int idx) { return m_ptr[idx]; }
		row& operator=(const_col const& other) { for (int i = 0; i < 4; i++) (*this)[i] = other[i]; return *this; }
	};

	struct const_row
	{
		const_row(float const* ptr) : m_ptr(ptr) {}
		float const* m_ptr;

		float operator[](int idx) const { return m_ptr[idx]; }
	};

	struct GridRow
	{
		float values[4];
	};

	struct GridView
	{
		GridRow rows[4];
	};

	struct Matrix4
	{
		Matrix4() { Clear(); /*gridview = new(raw)GridView;*/ }
		Matrix4(Matrix4 const& other) = default;
		Matrix4& operator=(const Matrix4& other) = default;
		Matrix4(float diagonalValue);
		~Matrix4() = default;

		/*
		 * @brief Create an identity matrix (diagonal == 1)
		 * @return a matrix4
		 */
		static Matrix4 Identity() { return Matrix4(1.f); }
		/*
		 * @brief Create a orthographic matrix
		 * @param 6 float : left, right, bot, top, near, far
		 * @return a matrix4
		 */
		static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);
		/*
		 * @brief Create a perspective matrix 
		 * @param Half of the fov in radians, float aspect ratio (width/height), float near and far
		 * @return a matrix4
		 */
		static Matrix4 Perspective(const Radian& halfFov, float aspectRatio, float near, float far);
		/*
		 * @brief Create a perspective matrix 
		 * @param Half of the fov in radians, float aspect ratio (width/height), float near and far
		 * @return a matrix4
		 */
		static Matrix4 PerspectiveLh(const Radian& halfFov, float aspectRatio, float near, float far);
		/*
		 * @brief Create a lookat matrix 
		 * @param 3 vector3 : eye of cam, position of cam, up vector
		 * @return a matrix4
		 */
		static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		/*
		 * @brief Create a lookat matrix left handed
		 * @param 3 vector3 : eye of cam, position of cam, up vector
		 * @return a matrix4
		 */
		static Matrix4 LookAtLh(const Vector3& eye, const Vector3& center, const Vector3& up);
		
		/*
		 * @brief Create a rotation matrix 
		 * @param quaternion
		 * @return a matrix4
		 */
		static Matrix4 Rotation(Quaternion quaternion);
		/*
		 * @brief Create a rotation matrix
		 * @param 3 radian x, y, z
		 * @return a matrix4
		 */
		static Matrix4 Rotation(Radian x, Radian y, Radian z);
		/*
		 * @brief Create a rotation matrix 
		 * @param vector with the 3 rotation components + bool is radian or degree
		 * @return a matrix4
		 */
		static Matrix4 Rotation(const Vector3& vec, bool radian);
		/*
		 * @brief Create a scaling matrix
		 * @param 3 floats with scaling components
		 * @return a matrix4
		 */
		static Matrix4 Scaling(float x, float y, float z);
		/*
		 * @brief Create a scaling matrix
		 * @param vector with the 3 scaling components
		 * @return a matrix4
		 */
		static Matrix4 Scaling(const Vector3& vec);
		/*
		 * @brief Create a translation matrix
		 * @param 3 floats with translation components
		 * @return a matrix4
		 */
		static Matrix4 Translation(float x, float y, float z);
		/*
		 * @brief Create a translation matrix
		 * @param vector with the 3 translation components
		 * @return a matrix4
		 */
		static Matrix4 Translation(const Vector3& vec);

		/*
		 * @brief Modifies a rotation matrix
		 * @param quaternion
		 * @return the modified matrix4
		 */
		 //Maybe swap order of multiplication
		Matrix4& Rotate(Quaternion const& quaternion) { *this = Rotation(quaternion) * (*this); return *this; }

		/*
		 * @brief Modifies a rotation matrix
		 * @param 3 radian x, y, z
		 * @return the modified matrix4
		 */
		Matrix4& Rotate(Radian x, Radian y, Radian z) { *this = Rotation(x, y, z)* (*this); return *this; }

		/*
		 * @brief Modifies a rotation matrix
		 * @param vector with the 3 rotation components + bool is radian or degree
		 * @return the modified matrix4
		 */
		Matrix4& Rotate(const Vector3& vec, bool radian) { *this = Rotation(vec, radian)* (*this); return *this; }

		/*
		 * @brief Modifies a scaling matrix
		 * @param 3 floats with scaling components
		 * @return the modified matrix4
		 */
		Matrix4& Scale(float x, float y, float z) { *this = Scaling(x, y, z)* (*this); return *this; }

		/*
		 * @brief Modifies a scaling matrix
		 * @param vector with the 3 scaling components
		 * @return the modified matrix4
		 */
		Matrix4& Scale(const Vector3& vec) { *this = Scaling(vec)* (*this); return *this; }

		/*
		 * @brief Modifies a translation matrix
		 * @param 3 floats with translation components
		 * @return the modified matrix4
		 */
		Matrix4& Translate(float x, float y, float z) { *this = Translation(x, y, z)* (*this); return *this; }

		/*
		 * @brief Modifies a translation matrix
		 * @param vector with the 3 translation components
		 * @return the modified matrix4
		 */
		Matrix4& Translate(const Vector3& vec) { *this = Translation(vec)* (*this); return *this; }

		bool operator==(const Matrix4& other) const;
		bool operator!=(const Matrix4& other) const { return !(*this == other); }

		row operator[](int idx) { return row(&raw[idx * 4]); }
		const_row operator[](int idx) const { return const_row(&raw[idx * 4]); }

		LibMath::col col(int idx) { return LibMath::col(&raw[idx]); }
		const_col col(int idx) const { return const_col(&raw[idx]); }

		Matrix4 operator*(Matrix4 const& other) const;
		Matrix4& operator*=(Matrix4 const& other);

		Vector4 operator*(Vector4 const& other) const;

		friend Vector4 operator*(Vector4 const& otherVec, Matrix4 const& thisMat);

		/*
		 * @brief called from the matrix which determinant we want
		 * @return a float determinant
		 */
		float Determinant() const;

		/*
		 * @brief called from the matrix you want the translation from
		 * @return this matrix now transposed
		 */
		[[nodiscard]] Vector4 GetTranslation() const { return Vector4((*this)[3][0], (*this)[3][1], (*this)[3][2], (*this)[3][3]); }
		static Vector4 GetTranslation(Matrix4& Matrix4) { return Matrix4.GetTranslation(); }

		/*
		 * @brief called from the matrix you want to transpose
		 * @return this matrix now transposed
		 */
		void Transpose() { *this = GetTranspose(); }
		[[nodiscard]] Matrix4 GetTranspose() const;
		static void Transpose(Matrix4& Matrix4) { Matrix4.Transpose(); }

		/*
		 * @brief called with the matrix you want the inverse from
		 * @return this matrix now inversed
		 */
		void Inverse() { *this = GetInverse(); }
		[[nodiscard]] Matrix4 GetInverse() const;
		static void Inverse(Matrix4& Matrix4) { Matrix4.Inverse(); }

		/*
		 * @brief called with the matrix you want the adjoint from
		 * @return this matrix now inversed
		 */
		void Adjoint() { *this = GetAdjoint(); }
		[[nodiscard]] Matrix4 GetAdjoint() const;
		static void Adjoint(Matrix4& Matrix4) { Matrix4.Adjoint(); }

		/*
		 * @brief called with the matrix you want to extract the normal from
		 * @return this normal matrix
		 */
		Matrix4 NormalMatrix4() const;
		static void NormalMatrix4(Matrix4 const& Matrix4);

		/*
		 * @brief erase all values from a matrix
		 */
		void Clear() { std::memset(&raw, 0, sizeof(raw)); }

		float const* Data() const { return raw; }

		float raw[16];
		//GridView* gridview;

	private:

		static float GetMinor(float p_minor0, float p_minor1, float p_minor2, float p_minor3, float p_minor4, float p_minor5, float p_minor6, float p_minor7, float p_minor8);
		float Determinant(int ignoredRow, int ignoredCol) const;
	};
}
