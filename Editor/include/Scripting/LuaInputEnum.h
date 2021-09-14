#pragma once
#include <String>
#include <unordered_map>

#include "Windowing/EInputGamepadAxis.h"
#include "Windowing/EInputGamepadButton.h"
#include "Windowing/EInputKeyboard.h"
#include "Windowing/EInputMouse.h"
#include "Windowing/EInputState.h"

namespace NIX::Editor
{
	class LuaInputEnum
	{
	public:

		static void Init();

		static Rendering::EInputGamepadAxis GetInputGamepadAxis(std::string inputGamepadAxisString);
		static Rendering::EInputGamepadButton GetInputGamepadButton(std::string inputGamepadButtonString);
		static Rendering::EInputKeyboard GetInputKeyboard(std::string inputKeyboardString);
		static Rendering::EInputMouse GetInputMouse(std::string inputMouseString);
		static Rendering::EInputState GetInputState(std::string inputStateString);


	private:

		inline static std::unordered_map<std::string, Rendering::EInputGamepadAxis> m_enumInputGamepadAxisMap;
		inline static std::unordered_map<std::string, Rendering::EInputGamepadButton> m_enumInputGamepadButtonMap;
		inline static std::unordered_map<std::string, Rendering::EInputKeyboard> m_enumInputKeyboardMap;
		inline static std::unordered_map<std::string, Rendering::EInputMouse> m_enumInputMouseMap;
		inline static std::unordered_map<std::string, Rendering::EInputState> m_enumInputStateMap;
	};

}
