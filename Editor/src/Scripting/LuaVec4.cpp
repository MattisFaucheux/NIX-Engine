#include "pch.h"
#include "Scripting/LuaVec4.h"

#include "Scripting/ScriptManager.h"
#include "Vector/Vector4.h"


namespace NIX::Editor
{

	void LuaVec4::LuaInit()
	{
		ScriptManager::GetSingleton()->AddClass<LibMath::Vector4, float, float, float, float>("Vector4");

		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector4>("Vector4", "X", &LibMath::Vector4::x, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector4>("Vector4", "Y", &LibMath::Vector4::y, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector4>("Vector4", "Z", &LibMath::Vector4::z, true);
		ScriptManager::GetSingleton()->AddClassData<LibMath::Vector4>("Vector4", "W", &LibMath::Vector4::w, true);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "__equal", &LibMath::Vector4::operator==);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "__add", &LibMath::Vector4::Add);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Add", &LibMath::Vector4::AddEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "__sub", &LibMath::Vector4::Sub);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Sub", &LibMath::Vector4::SubEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "__mul", &LibMath::Vector4::Multiply);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Multiply", &LibMath::Vector4::MultiplyEqual);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "__div", &LibMath::Vector4::Divide);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Divide", &LibMath::Vector4::DivideEqual);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "IsDirection", &LibMath::Vector4::IsDirection);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "IsPoint", &LibMath::Vector4::IsPoint);

		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Dot", &LibMath::Vector4::Dot);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "IsHomogenize", &LibMath::Vector4::IsHomogenize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "Homogenize", &LibMath::Vector4::Homogenize);
		ScriptManager::GetSingleton()->AddClassFunction<LibMath::Vector4>("Vector4", "GetHomogenize", &LibMath::Vector4::GetHomogenize);
	}
}