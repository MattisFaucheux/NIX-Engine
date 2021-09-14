#include "pch.h"
#include "Scripting/LuaVec3.h"

#include "Scripting/ScriptManager.h"
#include "Vector/Vector3.h"


namespace NIX::Editor
{

	void LuaVec3::LuaInit()
	{

		ScriptManager::GetSingleton()->AddClass<LibMath::Vector3, float, float, float>("Vector3");

		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector3>("Vector3", "X", &LibMath::Vector3::x, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector3>("Vector3", "Y", &LibMath::Vector3::y, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector3>("Vector3", "Z", &LibMath::Vector3::z, true);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "__equal", &LibMath::Vector3::operator==);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "__add", &LibMath::Vector3::Add);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Add", &LibMath::Vector3::AddEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "__sub", &LibMath::Vector3::Sub);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Sub", &LibMath::Vector3::SubEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "__mul", &LibMath::Vector3::Multiply);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Multiply", &LibMath::Vector3::MultiplyEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "__div", &LibMath::Vector3::Divide);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Divide", &LibMath::Vector3::DivideEqual);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Dot", &LibMath::Vector3::Dot);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Cross", &LibMath::Vector3::Cross);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Magnitude", &LibMath::Vector3::Magnitude);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "SquareMagnitude", &LibMath::Vector3::SquareMagnitude);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "IsShorterThan", &LibMath::Vector3::IsShorterThan);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "IsShorterOrEqualTo", &LibMath::Vector3::IsShorterOrEqualTo);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "IsLongerThan", &LibMath::Vector3::IsLongerThan);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "IsLongerOrEqualTo", &LibMath::Vector3::IsLongerOrEqualTo);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "IsNormalize", &LibMath::Vector3::IsNormalize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "Normalize", &LibMath::Vector3::Normalize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "GetNormalize", &LibMath::Vector3::GetNormalize);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector3>("Vector3", "DistanceTo", &LibMath::Vector3::DistanceTo);


	}
}
