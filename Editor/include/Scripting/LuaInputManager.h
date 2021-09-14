#pragma once

#include <string>

namespace NIX::Editor
{
	class LuaInputManager
	{
	public:

		static void LuaInit();

	private:

		static bool IsKeyboardInputReleased(std::string key);
		static bool IsKeyboardInputPressed(std::string key);
		static bool IsKeyboardInputRepeated(std::string key);

		static bool IsMouseInputReleased(std::string key);
		static bool IsMouseInputRepeated(std::string key);

		static bool IsGamepadInputReleased(int jid, std::string key);
		static bool IsGamepadInputRepeated(int jid, std::string key);
		static float GetGamepadInputAxis(int jid, std::string axis);

		static bool IsInputReleased(std::string inputName);
		static bool IsInputPressed(std::string inputName);
		static bool IsInputRepeated(std::string inputName);
		static float GetInputAxis(std::string inputName);

		static bool AddKeyboardInputBind(std::string inputName, std::string key);
		static bool AddMouseInputBind(std::string inputName, std::string key);
		static bool AddGamepadInputBind(std::string inputName, int jid, std::string key);
		static bool AddGamepadAxisInputBind(std::string inputName, int jid, std::string key);

		static double GetCursorPosX();
		static double GetCursorPosY();

		static bool RemoveInputBind(std::string inputName);
	};

}
