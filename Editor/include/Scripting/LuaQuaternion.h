#pragma once
#include "Quaternion/Quaternion.h"

namespace LibMath
{
	struct Quaternion;
}

namespace NIX::Editor
{
	class LuaQuaternion
	{
	public:

		static void LuaInit();

		static LibMath::Quaternion NewQuaternion(float angle, LibMath::Vector3& axis);
	};

}
