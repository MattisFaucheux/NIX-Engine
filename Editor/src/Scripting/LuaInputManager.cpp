#include "pch.h"
#include "Scripting/LuaInputManager.h"

#include "Scripting/ScriptManager.h"
#include "Windowing/InputManager.h"

#include "Scripting/LuaInputEnum.h"

namespace NIX::Editor
{
	void LuaInputManager::LuaInit()
	{
		LuaInputEnum::Init();

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsKeyboardInputReleased", LuaInputManager::IsKeyboardInputReleased);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsKeyboardInputPressed", LuaInputManager::IsKeyboardInputPressed);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsKeyboardInputRepeated", LuaInputManager::IsKeyboardInputRepeated);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsMouseInputReleased", LuaInputManager::IsMouseInputReleased);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsMouseInputRepeated", LuaInputManager::IsMouseInputRepeated);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "GetCursorPosX", LuaInputManager::GetCursorPosX);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "GetCursorPosY", LuaInputManager::GetCursorPosY);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsGamepadInputReleased", LuaInputManager::IsGamepadInputReleased);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsGamepadInputRepeated", LuaInputManager::IsGamepadInputRepeated);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "GetGamepadInputAxis", LuaInputManager::GetGamepadInputAxis);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsInputReleased", LuaInputManager::IsInputReleased);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsInputPressed", LuaInputManager::IsInputPressed);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "IsInputRepeated", LuaInputManager::IsInputRepeated);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "GetInputAxis", LuaInputManager::GetInputAxis);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "AddKeyboardInputBind", LuaInputManager::AddKeyboardInputBind);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "AddMouseInputBind", LuaInputManager::AddMouseInputBind);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "AddGamepadInputBind", LuaInputManager::AddGamepadInputBind);
		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "AddGamepadAxisInputBind", LuaInputManager::AddGamepadAxisInputBind);

		ScriptManager::GetSingleton()->AddFunctionInNamespace("Input", "RemoveInputBind", LuaInputManager::RemoveInputBind);
	}

	bool LuaInputManager::IsKeyboardInputReleased(std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputReleased(LuaInputEnum::GetInputKeyboard(key));
	}

	bool LuaInputManager::IsKeyboardInputPressed(std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputPressed(LuaInputEnum::GetInputKeyboard(key));
	}

	bool LuaInputManager::IsKeyboardInputRepeated(std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputRepeated(LuaInputEnum::GetInputKeyboard(key));
	}

	bool LuaInputManager::IsMouseInputReleased(std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputReleased(LuaInputEnum::GetInputMouse(key));
	}

	bool LuaInputManager::IsMouseInputRepeated(std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputRepeated(LuaInputEnum::GetInputMouse(key));
	}

	bool LuaInputManager::IsGamepadInputReleased(int jid, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputReleased(jid, LuaInputEnum::GetInputGamepadButton(key));
	}

	bool LuaInputManager::IsGamepadInputRepeated(int jid, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->IsInputRepeated(jid, LuaInputEnum::GetInputGamepadButton(key));
	}

	float LuaInputManager::GetGamepadInputAxis(int jid, std::string axis)
	{
		return Rendering::InputManager::GetSingleton()->GetInputAxis(jid, LuaInputEnum::GetInputGamepadAxis(axis));
	}

	bool LuaInputManager::IsInputReleased(std::string inputName)
	{
		return Rendering::InputManager::GetSingleton()->IsInputReleased(inputName);
	}

	bool LuaInputManager::IsInputPressed(std::string inputName)
	{
		return Rendering::InputManager::GetSingleton()->IsInputPressed(inputName);
	}

	bool LuaInputManager::IsInputRepeated(std::string inputName)
	{
		return Rendering::InputManager::GetSingleton()->IsInputRepeated(inputName);
	}

	float LuaInputManager::GetInputAxis(std::string inputName)
	{
		return Rendering::InputManager::GetSingleton()->GetInputAxis(inputName);
	}

	bool LuaInputManager::AddKeyboardInputBind(std::string inputName, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->AddInputBind(inputName, LuaInputEnum::GetInputKeyboard(key));
	}

	bool LuaInputManager::AddMouseInputBind(std::string inputName, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->AddInputBind(inputName, LuaInputEnum::GetInputMouse(key));
	}

	bool LuaInputManager::AddGamepadInputBind(std::string inputName, int jid, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->AddInputBind(inputName, jid, LuaInputEnum::GetInputGamepadButton(key));
	}

	bool LuaInputManager::AddGamepadAxisInputBind(std::string inputName, int jid, std::string key)
	{
		return Rendering::InputManager::GetSingleton()->AddInputBind(inputName, jid, LuaInputEnum::GetInputGamepadAxis(key));
	}

	double LuaInputManager::GetCursorPosX()
	{
		return Rendering::InputManager::GetSingleton()->GetCursorPosX();
	}

	double LuaInputManager::GetCursorPosY()
	{
		return Rendering::InputManager::GetSingleton()->GetCursorPosY();
	}

	bool LuaInputManager::RemoveInputBind(std::string inputName)
	{
		return Rendering::InputManager::GetSingleton()->RemoveInputBind(inputName);
	}
}
