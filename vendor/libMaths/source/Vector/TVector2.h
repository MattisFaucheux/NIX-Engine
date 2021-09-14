#pragma once

#include "Core/CMath.h"
#include <cmath>
#include <cfloat>

#include "Core/Angle.h"

namespace LibMath
{
	// TODO implement move operator and move constructor
	/**
	* A structure to represent 2d vector
	*/
	template<typename T>
	struct TVector2
	{
		/*
		* @name Coordinates
		*/
		/*@{*/
		T x = 0;/**< the x coordinate*/
		T y = 0;/**< the y coordinate*/
		/*@}*/

		/**
		* @brief Default constructor generate a vector (0,0)
		*/
		TVector2() = default;

		/**
		* @brief Default destructor
		*/
		~TVector2() = default;

		/**
		* @brief Default copy Constructor
		*
		* @param other 2D Vector to copy from.
		*/
		TVector2(const TVector2& other) = default;

		/**
		 * @brief Default assignment operator
		 * @param other Vector to copy
		 */
		TVector2& operator=(const TVector2& other) = default;

		/**
		* @brief Constructor initializing all components to a single float value.
		*
		* @param value Value to set all components to.
		*/

		explicit TVector2(const T value) : x(value), y(value) {}

		/**
		* @brief Constructor using initial values for each component.
		*
		* @param px X Coordinate.
		* @param py Y Coordinate.
		*/
		explicit TVector2(const T px, const T py) : x(px), y(py) {}

		/**
		* @brief Check against another vector for equality.
		*
		* @param other The vector to check against.
		* @return true if the vectors are equal, false otherwise.
		*/
		[[nodiscard]] bool operator==(const TVector2<T>& other) const { return x == other.x && y == other.y; }

		/**
		* @brief Check against another vector for inequality.
		*
		* @param other The vector to check against.
		* @return true if the vectors are not equal, false otherwise.
		*/
		[[nodiscard]] bool operator!=(const TVector2<T>& other) const { return !(*this == other); }

		/**
		 * @brief Compare both vector with a tolerance (default tolerance is 1.192092896e-07F)
		 *
		 * @param other Vector to compare with
		 * @param tolerance Tolerance between coordinates
		 * @return true if both vector are equal, false if not
		 */
		[[nodiscard]] bool Equals(const TVector2<T>& other, const T tolerance = FLT_EPSILON) const { return Absolute(x - other.x) <= tolerance && Absolute(y - other.y) <= tolerance; }

		/**
		* @brief Calculate the Magnitude.
		*
		* @return Magnitude of vector.
		*/
		[[nodiscard]] T Magnitude() const
		{
			return std::sqrt(x * x + y * y);
		}

		/**
		* @brief This function is more optimize because does not have Square.
		*
		* @return Square Magnitude.
		*/
		[[nodiscard]] T SquareMagnitude() const { return x * x + y * y; }

		/**
		 * @brief Compare two vector length
		 * @param other Second vector to compare
		 * @return True if the first vector is longer, false if not
		 */
		[[nodiscard]] bool IsLonger(const TVector2<T>& other) const { return SquareMagnitude() > other.SquareMagnitude(); }

		/**
		 * @brief Compare two vector length
		 * @param other Second vector to compare
		 * @return True if the first vector is longer or equal, false if not
		 */
		[[nodiscard]] bool IsLongerOrEqual(const TVector2<T>& other) const { return !this->IsShorter(other); }

		/**
		 * @brief Compare two vector length
		 * @param other Second vector to compare
		 * @return True if the first vector is shorter, false if not
		 */
		[[nodiscard]] bool IsShorter(const TVector2<T>& other) const { return SquareMagnitude() < other.SquareMagnitude(); }

		/**
		 * @brief Compare two vector length
		 * @param other Second vector to compare
		 * @return True if the first vector is shorter or equal, false if not
		 */
		[[nodiscard]] bool IsShorterOrEqual(const TVector2<T>& other) const { return !this->IsLonger(other); }

		/**
		* @brief Gets the result of component-wise addition of this and another vector.
		*
		* @param rhs The vector to add to this.
		* @return The result of vector addition.
		*/
		TVector2<T>& operator+=(const TVector2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }

		/**
		* @brief Gets the result of component-wise subtraction of this by another vector.
		*
		* @param rhs The vector to subtract from this.
		* @return The result of vector subtraction.
		*/
		TVector2<T>& operator-=(const TVector2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

		/**
		* @brief Multiply the vector by three different value
		*
		* @param rhs Ratio to use
		* @return The result of multiplication.
		*/
		TVector2<T>& operator*=(const TVector2<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }

		/**
		* @brief Divide the vector by three different value
		*
		* @param rhs Ratio to use
		* @return The result of division.
		*/
		TVector2<T>& operator/=(const TVector2<T>& rhs)
		{
			// TODO Optimize to only have check in debug
			if (rhs.x == 0 || rhs.y == 0)
			{
				x = 0;
				y = 0;
				return *this;
			}

			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}

		/**
		* @brief Get a negated copy of the vector.
		* @param rhs The negated vector
		* @return A negated copy of the vector.
		*/
		friend TVector2<T> operator-(const TVector2<T>& rhs) { return TVector2<T>(-rhs.x, -rhs.y); }

		/**
		* @brief  Subtract two TVector2 together.
		* @param lhs TVector2 on the left side of the operator.
		* @param rhs TVector2 on the right side of the operator.
		* @return A TVector2 holding the sum of the subtraction
		*/
		friend TVector2<T> operator-(TVector2<T> lhs, const TVector2<T>& rhs) { lhs -= rhs; return lhs; }

		/**
		* @brief  Add two TVector2 together.
		* @param lhs TVector2 on the left side of the operator.
		* @param rhs TVector2 on the right side of the operator.
		* @return A TVector2 holding the sum of the addition
		*/
		friend TVector2<T> operator+(TVector2<T> lhs, const TVector2<T>& rhs) { lhs += rhs; return lhs; }

		/**
		* @brief  Multiply two TVector2 together(x*x, y*y).
		* @param lhs TVector2 on the left side of the operator.
		* @param rhs TVector2 on the right side of the operator.
		* @return A TVector2 holding the sum of the multiplication
		*/
		friend TVector2<T> operator*(TVector2<T> lhs, const TVector2<T>& rhs) { lhs *= rhs; return lhs; }

		/**
		* @brief  Divide two TVector2 together(x/x, y/y).
		* @param lhs TVector2 on the left side of the operator.
		* @param rhs TVector2 on the right side of the operator.
		* @return A TVector2 holding the sum of the multiplication
		*/
		friend TVector2<T> operator/(TVector2<T> lhs, const TVector2<T>& rhs) { lhs /= rhs; return lhs; }

		/**
		* @brief Calculate the dot product of two vectors.
		*
		* @param rhs The first vector.
		* @return The dot product.
		*/
		[[nodiscard]] float Dot(const TVector2<T>& rhs) const { return x * rhs.x + y * rhs.y; }

		/**
		* @brief Checks whether vector is normalized.
		*
		* @return true if normalized, false otherwise.
		*/
		[[nodiscard]] bool IsNormalize() const
		{
			const float sqrtMagnitude = SquareMagnitude();
			return 0.9999995f < sqrtMagnitude && sqrtMagnitude < 1.0000005f;
		}

		/** @brief Normalize a vector */
		void Normalize()
		{
			float magnitude = Magnitude();
			x /= magnitude;
			y /= magnitude;
		}

		/**
		 * @brief Normalize a Vector
		 * @return a normalize copy of the vector
		 */
		[[nodiscard]] TVector2<T> GetNormalize() const { float size = Magnitude(); return TVector2<T>(x / size, y / size); }

		//TODO review all under
		/**
		* @brief Euclidean distance between two points.
		*
		* @param lhs The first point.
		* @param rhs The second point.
		* @return The distance between two points.
		*/
		[[nodiscard]] static float DistanceBetween(const TVector2<T>& lhs, const TVector2<T>& rhs)
		{
			return (lhs - rhs).Magnitude();
		}

		/**
		* @brief Euclidean distance between two points.
		*
		* @param other The Other point.
		* @return The distance between two points.
		*/
		[[nodiscard]] float DistanceTo(const TVector2<T>& other) const
		{
			return DistanceBetween(*this, other);
		}

		/**
		* @brief Extract angle between two vectors.
		*
		* @param firstVector The first point.
		* @param secondVector The second point.
		* @return A angle in Radian.
		*/
		[[nodiscard]] static Radian ExtractAngleBetween(TVector2<T> firstVector, TVector2<T> secondVector)
		{
			const float firstVectorMagnitude2 = firstVector.SquareMagnitude();
			if (firstVectorMagnitude2 != 1.f)
			{
				float firstVectorMagnitude = std::sqrt(firstVectorMagnitude2);
				firstVector.x /= firstVectorMagnitude;
				firstVector.y /= firstVectorMagnitude;
			}

			const float secondVectorMagnitude2 = secondVector.SquareMagnitude();
			if (secondVectorMagnitude2 != 1.f)
			{
				float secondVectorMagnitude = std::sqrt(secondVectorMagnitude2);
				secondVector.x /= secondVectorMagnitude;
				secondVector.y /= secondVectorMagnitude;
			}

			return Radian(std::acos(firstVector.Dot(secondVector)));
		}

		/**
		* @brief Get angle between two Unit vectors.
		*
		* @param firstVector The first point.
		* @param secondVector The second point.
		* @return A angle in Radian.
		*/
		[[nodiscard]] static Radian GetAngleBetweenUnitVector(const TVector2<T>& firstVector, const TVector2<T>& secondVector)
		{
			return Radian(std::acos(firstVector.Dot(secondVector)));
		}

		//TODO Comment why this is useful
		float& operator[](const int index)
		{
			return static_cast<float*>(this)[index];
		}

		//TODO Comment why this is useful
		float operator[](const int index) const
		{
			return static_cast<float*>(this)[index];
		}
	};

	typedef TVector2<float> Vector2;
	typedef TVector2<int> Vector2i;
}
