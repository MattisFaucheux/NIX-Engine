#include "Scripting\LuaQuaternion.h"

#include "Scripting/ScriptManager.h"

namespace NIX::Editor
{
	void LuaQuaternion::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<LibMath::Quaternion, float, float, float, float>("Quaternion");

		ScriptManager::GetSingleton()->AddClassData<LibMath::Quaternion>("Quaternion", "X", &LibMath::Quaternion::X, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Quaternion>("Quaternion", "Y", &LibMath::Quaternion::Y, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Quaternion>("Quaternion", "Z", &LibMath::Quaternion::Z, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Quaternion>("Quaternion", "W", &LibMath::Quaternion::W, true);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "__equal", &LibMath::Quaternion::operator==);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "__mul", &LibMath::Quaternion::Multiply);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "MultiplyEqual", &LibMath::Quaternion::MultiplyEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "MultiplyF", &LibMath::Quaternion::MultiplyF);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "MultiplyEqualF", &LibMath::Quaternion::MultiplyEqualF);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "GetOffset", &LibMath::Quaternion::GetOffset);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "EulerAngles", &LibMath::Quaternion::EulerAngles);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Norm", &LibMath::Quaternion::Norm);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "GetInverse", &LibMath::Quaternion::GetInverse);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Negate", &LibMath::Quaternion::Negate);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Inverse", &LibMath::Quaternion::Inverse);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Conjugate", &LibMath::Quaternion::Conjugate);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "GetNormalize", &LibMath::Quaternion::GetNormalize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Normalize", &LibMath::Quaternion::Normalize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "DotProduct", &LibMath::Quaternion::DotProduct);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Quaternion>("Quaternion", "Rotate", &LibMath::Quaternion::Rotate);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Quaternion", "New", LuaQuaternion::NewQuaternion);
	}

	LibMath::Quaternion LuaQuaternion::NewQuaternion(float angle, LibMath::Vector3& axis)
	{

		return LibMath::Quaternion(LibMath::Degree(angle), axis);
	}

}
