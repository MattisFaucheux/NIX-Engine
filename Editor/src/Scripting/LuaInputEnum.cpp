#include "Log/Logger.h"
#include "Scripting/LuaInputEnum.h"

namespace NIX::Editor
{



	void LuaInputEnum::Init()
	{
		m_enumInputGamepadAxisMap["LEFT_X"] = Rendering::EInputGamepadAxis::LEFT_X;
		m_enumInputGamepadAxisMap["LEFT_Y"] = Rendering::EInputGamepadAxis::LEFT_Y;
		m_enumInputGamepadAxisMap["RIGHT_X"] = Rendering::EInputGamepadAxis::RIGHT_X;
		m_enumInputGamepadAxisMap["RIGHT_Y"] = Rendering::EInputGamepadAxis::RIGHT_Y;
		m_enumInputGamepadAxisMap["LEFT_TRIGGER"] = Rendering::EInputGamepadAxis::LEFT_TRIGGER;
		m_enumInputGamepadAxisMap["RIGHT_TRIGGER"] = Rendering::EInputGamepadAxis::RIGHT_TRIGGER;

		m_enumInputGamepadButtonMap["A"] = Rendering::EInputGamepadButton::A;
		m_enumInputGamepadButtonMap["B"] = Rendering::EInputGamepadButton::B;
		m_enumInputGamepadButtonMap["X"] = Rendering::EInputGamepadButton::X;
		m_enumInputGamepadButtonMap["Y"] = Rendering::EInputGamepadButton::Y;
		m_enumInputGamepadButtonMap["LEFT_BUMPER"] = Rendering::EInputGamepadButton::LEFT_BUMPER;
		m_enumInputGamepadButtonMap["RIGHT_BUMPER"] = Rendering::EInputGamepadButton::RIGHT_BUMPER;
		m_enumInputGamepadButtonMap["BACK"] = Rendering::EInputGamepadButton::BACK;
		m_enumInputGamepadButtonMap["START"] = Rendering::EInputGamepadButton::START;
		m_enumInputGamepadButtonMap["GUIDE"] = Rendering::EInputGamepadButton::GUIDE;
		m_enumInputGamepadButtonMap["LEFT_THUMB"] = Rendering::EInputGamepadButton::LEFT_THUMB;
		m_enumInputGamepadButtonMap["RIGHT_THUMB"] = Rendering::EInputGamepadButton::RIGHT_THUMB;
		m_enumInputGamepadButtonMap["DPAD_UP"] = Rendering::EInputGamepadButton::DPAD_UP;
		m_enumInputGamepadButtonMap["DPAD_RIGHT"] = Rendering::EInputGamepadButton::DPAD_RIGHT;
		m_enumInputGamepadButtonMap["DPAD_DOWN"] = Rendering::EInputGamepadButton::DPAD_DOWN;
		m_enumInputGamepadButtonMap["DPAD_LEFT"] = Rendering::EInputGamepadButton::DPAD_LEFT;

		m_enumInputKeyboardMap["SPACE"] = Rendering::EInputKeyboard::SPACE;
		m_enumInputKeyboardMap["APOSTROPHE"] = Rendering::EInputKeyboard::APOSTROPHE;
		m_enumInputKeyboardMap["COMMA"] = Rendering::EInputKeyboard::COMMA;
		m_enumInputKeyboardMap["MINUS"] = Rendering::EInputKeyboard::MINUS;
		m_enumInputKeyboardMap["PERIOD"] = Rendering::EInputKeyboard::PERIOD;
		m_enumInputKeyboardMap["SLASH"] = Rendering::EInputKeyboard::SLASH;
		m_enumInputKeyboardMap["NUM_0"] = Rendering::EInputKeyboard::NUM_0;
		m_enumInputKeyboardMap["NUM_1"] = Rendering::EInputKeyboard::NUM_1;
		m_enumInputKeyboardMap["NUM_2"] = Rendering::EInputKeyboard::NUM_2;
		m_enumInputKeyboardMap["NUM_3"] = Rendering::EInputKeyboard::NUM_3;
		m_enumInputKeyboardMap["NUM_4"] = Rendering::EInputKeyboard::NUM_4;
		m_enumInputKeyboardMap["NUM_5"] = Rendering::EInputKeyboard::NUM_5;
		m_enumInputKeyboardMap["NUM_6"] = Rendering::EInputKeyboard::NUM_6;
		m_enumInputKeyboardMap["NUM_7"] = Rendering::EInputKeyboard::NUM_7;
		m_enumInputKeyboardMap["NUM_8"] = Rendering::EInputKeyboard::NUM_8;
		m_enumInputKeyboardMap["NUM_9"] = Rendering::EInputKeyboard::NUM_9;
		m_enumInputKeyboardMap["SEMICOLON"] = Rendering::EInputKeyboard::SEMICOLON;
		m_enumInputKeyboardMap["EQUAL"] = Rendering::EInputKeyboard::EQUAL;
		m_enumInputKeyboardMap["A"] = Rendering::EInputKeyboard::A;
		m_enumInputKeyboardMap["B"] = Rendering::EInputKeyboard::B;
		m_enumInputKeyboardMap["C"] = Rendering::EInputKeyboard::C;
		m_enumInputKeyboardMap["D"] = Rendering::EInputKeyboard::D;
		m_enumInputKeyboardMap["E"] = Rendering::EInputKeyboard::E;
		m_enumInputKeyboardMap["F"] = Rendering::EInputKeyboard::F;
		m_enumInputKeyboardMap["G"] = Rendering::EInputKeyboard::G;
		m_enumInputKeyboardMap["H"] = Rendering::EInputKeyboard::H;
		m_enumInputKeyboardMap["I"] = Rendering::EInputKeyboard::I;
		m_enumInputKeyboardMap["J"] = Rendering::EInputKeyboard::J;
		m_enumInputKeyboardMap["K"] = Rendering::EInputKeyboard::K;
		m_enumInputKeyboardMap["L"] = Rendering::EInputKeyboard::L;
		m_enumInputKeyboardMap["M"] = Rendering::EInputKeyboard::M;
		m_enumInputKeyboardMap["N"] = Rendering::EInputKeyboard::N;
		m_enumInputKeyboardMap["O"] = Rendering::EInputKeyboard::O;
		m_enumInputKeyboardMap["P"] = Rendering::EInputKeyboard::P;
		m_enumInputKeyboardMap["Q"] = Rendering::EInputKeyboard::Q;
		m_enumInputKeyboardMap["R"] = Rendering::EInputKeyboard::R;
		m_enumInputKeyboardMap["S"] = Rendering::EInputKeyboard::S;
		m_enumInputKeyboardMap["T"] = Rendering::EInputKeyboard::T;
		m_enumInputKeyboardMap["U"] = Rendering::EInputKeyboard::U;
		m_enumInputKeyboardMap["V"] = Rendering::EInputKeyboard::V;
		m_enumInputKeyboardMap["W"] = Rendering::EInputKeyboard::W;
		m_enumInputKeyboardMap["X"] = Rendering::EInputKeyboard::X;
		m_enumInputKeyboardMap["Y"] = Rendering::EInputKeyboard::Y;
		m_enumInputKeyboardMap["Z"] = Rendering::EInputKeyboard::Z;
		m_enumInputKeyboardMap["LEFT_BRACKET"] = Rendering::EInputKeyboard::LEFT_BRACKET;
		m_enumInputKeyboardMap["BACKSLASH"] = Rendering::EInputKeyboard::BACKSLASH;
		m_enumInputKeyboardMap["RIGHT_BRACKET"] = Rendering::EInputKeyboard::RIGHT_BRACKET;
		m_enumInputKeyboardMap["GRAVE_ACCENT"] = Rendering::EInputKeyboard::GRAVE_ACCENT;
		m_enumInputKeyboardMap["WORLD_1"] = Rendering::EInputKeyboard::WORLD_1;
		m_enumInputKeyboardMap["WORLD_2"] = Rendering::EInputKeyboard::WORLD_2;
		m_enumInputKeyboardMap["ESCAPE"] = Rendering::EInputKeyboard::ESCAPE;
		m_enumInputKeyboardMap["ENTER"] = Rendering::EInputKeyboard::ENTER;
		m_enumInputKeyboardMap["TAB"] = Rendering::EInputKeyboard::TAB;
		m_enumInputKeyboardMap["BACKSPACE"] = Rendering::EInputKeyboard::BACKSPACE;
		m_enumInputKeyboardMap["INSERT"] = Rendering::EInputKeyboard::INSERT;
		m_enumInputKeyboardMap["DEL"] = Rendering::EInputKeyboard::DEL;
		m_enumInputKeyboardMap["RIGHT"] = Rendering::EInputKeyboard::RIGHT;
		m_enumInputKeyboardMap["LEFT"] = Rendering::EInputKeyboard::LEFT;
		m_enumInputKeyboardMap["DOWN"] = Rendering::EInputKeyboard::DOWN;
		m_enumInputKeyboardMap["UP"] = Rendering::EInputKeyboard::UP;
		m_enumInputKeyboardMap["PAGE_UP"] = Rendering::EInputKeyboard::PAGE_UP;
		m_enumInputKeyboardMap["PAGE_DOWN"] = Rendering::EInputKeyboard::PAGE_DOWN;
		m_enumInputKeyboardMap["HOME"] = Rendering::EInputKeyboard::HOME;
		m_enumInputKeyboardMap["END"] = Rendering::EInputKeyboard::END;
		m_enumInputKeyboardMap["CAPS_LOCK"] = Rendering::EInputKeyboard::CAPS_LOCK;
		m_enumInputKeyboardMap["SCROLL_LOCK"] = Rendering::EInputKeyboard::SCROLL_LOCK;
		m_enumInputKeyboardMap["NUM_LOCK"] = Rendering::EInputKeyboard::NUM_LOCK;
		m_enumInputKeyboardMap["PRINT_SCREEN"] = Rendering::EInputKeyboard::PRINT_SCREEN;
		m_enumInputKeyboardMap["PAUSE"] = Rendering::EInputKeyboard::PAUSE;
		m_enumInputKeyboardMap["F1"] = Rendering::EInputKeyboard::F1;
		m_enumInputKeyboardMap["F2"] = Rendering::EInputKeyboard::F2;
		m_enumInputKeyboardMap["F3"] = Rendering::EInputKeyboard::F3;
		m_enumInputKeyboardMap["F4"] = Rendering::EInputKeyboard::F4;
		m_enumInputKeyboardMap["F5"] = Rendering::EInputKeyboard::F5;
		m_enumInputKeyboardMap["F6"] = Rendering::EInputKeyboard::F6;
		m_enumInputKeyboardMap["F7"] = Rendering::EInputKeyboard::F7;
		m_enumInputKeyboardMap["F8"] = Rendering::EInputKeyboard::F8;
		m_enumInputKeyboardMap["F9"] = Rendering::EInputKeyboard::F9;
		m_enumInputKeyboardMap["F10"] = Rendering::EInputKeyboard::F10;
		m_enumInputKeyboardMap["F11"] = Rendering::EInputKeyboard::F11;
		m_enumInputKeyboardMap["F12"] = Rendering::EInputKeyboard::F12;
		m_enumInputKeyboardMap["F13"] = Rendering::EInputKeyboard::F13;
		m_enumInputKeyboardMap["F14"] = Rendering::EInputKeyboard::F14;
		m_enumInputKeyboardMap["F15"] = Rendering::EInputKeyboard::F15;
		m_enumInputKeyboardMap["F16"] = Rendering::EInputKeyboard::F16;
		m_enumInputKeyboardMap["F17"] = Rendering::EInputKeyboard::F17;
		m_enumInputKeyboardMap["F18"] = Rendering::EInputKeyboard::F18;
		m_enumInputKeyboardMap["F19"] = Rendering::EInputKeyboard::F19;
		m_enumInputKeyboardMap["F20"] = Rendering::EInputKeyboard::F20;
		m_enumInputKeyboardMap["F21"] = Rendering::EInputKeyboard::F21;
		m_enumInputKeyboardMap["F22"] = Rendering::EInputKeyboard::F22;
		m_enumInputKeyboardMap["F23"] = Rendering::EInputKeyboard::F23;
		m_enumInputKeyboardMap["F24"] = Rendering::EInputKeyboard::F24;
		m_enumInputKeyboardMap["F25"] = Rendering::EInputKeyboard::F25;
		m_enumInputKeyboardMap["KP_0"] = Rendering::EInputKeyboard::KP_0;
		m_enumInputKeyboardMap["KP_1"] = Rendering::EInputKeyboard::KP_1;
		m_enumInputKeyboardMap["KP_2"] = Rendering::EInputKeyboard::KP_2;
		m_enumInputKeyboardMap["KP_3"] = Rendering::EInputKeyboard::KP_3;
		m_enumInputKeyboardMap["KP_4"] = Rendering::EInputKeyboard::KP_4;
		m_enumInputKeyboardMap["KP_5"] = Rendering::EInputKeyboard::KP_5;
		m_enumInputKeyboardMap["KP_6"] = Rendering::EInputKeyboard::KP_6;
		m_enumInputKeyboardMap["KP_7"] = Rendering::EInputKeyboard::KP_7;
		m_enumInputKeyboardMap["KP_8"] = Rendering::EInputKeyboard::KP_8;
		m_enumInputKeyboardMap["KP_9"] = Rendering::EInputKeyboard::KP_9;
		m_enumInputKeyboardMap["KP_DECIMAL"] = Rendering::EInputKeyboard::KP_DECIMAL;
		m_enumInputKeyboardMap["KP_DIVIDE"] = Rendering::EInputKeyboard::KP_DIVIDE;
		m_enumInputKeyboardMap["KP_MULTIPLY"] = Rendering::EInputKeyboard::KP_MULTIPLY;
		m_enumInputKeyboardMap["KP_SUBTRACT"] = Rendering::EInputKeyboard::KP_SUBTRACT;
		m_enumInputKeyboardMap["KP_ADD"] = Rendering::EInputKeyboard::KP_ADD;
		m_enumInputKeyboardMap["KP_ENTER"] = Rendering::EInputKeyboard::KP_ENTER;
		m_enumInputKeyboardMap["KP_EQUAL"] = Rendering::EInputKeyboard::KP_EQUAL;
		m_enumInputKeyboardMap["LEFT_SHIFT"] = Rendering::EInputKeyboard::LEFT_SHIFT;
		m_enumInputKeyboardMap["LEFT_CONTROL"] = Rendering::EInputKeyboard::LEFT_CONTROL;
		m_enumInputKeyboardMap["LEFT_ALT"] = Rendering::EInputKeyboard::LEFT_ALT;
		m_enumInputKeyboardMap["LEFT_SUPER"] = Rendering::EInputKeyboard::LEFT_SUPER;
		m_enumInputKeyboardMap["RIGHT_SHIFT"] = Rendering::EInputKeyboard::RIGHT_SHIFT;
		m_enumInputKeyboardMap["RIGHT_CONTROL"] = Rendering::EInputKeyboard::RIGHT_CONTROL;
		m_enumInputKeyboardMap["RIGHT_ALT"] = Rendering::EInputKeyboard::RIGHT_ALT;
		m_enumInputKeyboardMap["RIGHT_SUPER"] = Rendering::EInputKeyboard::RIGHT_SUPER;
		m_enumInputKeyboardMap["MENU"] = Rendering::EInputKeyboard::MENU;

		m_enumInputMouseMap["BUTTON_LEFT"] = Rendering::EInputMouse::BUTTON_LEFT;
		m_enumInputMouseMap["BUTTON_RIGHT"] = Rendering::EInputMouse::BUTTON_RIGHT;
		m_enumInputMouseMap["BUTTON_MIDDLE"] = Rendering::EInputMouse::BUTTON_MIDDLE;
		m_enumInputMouseMap["BUTTON_4"] = Rendering::EInputMouse::BUTTON_4;
		m_enumInputMouseMap["BUTTON_5"] = Rendering::EInputMouse::BUTTON_5;
		m_enumInputMouseMap["BUTTON_6"] = Rendering::EInputMouse::BUTTON_6;
		m_enumInputMouseMap["BUTTON_7"] = Rendering::EInputMouse::BUTTON_7;
		m_enumInputMouseMap["BUTTON_8"] = Rendering::EInputMouse::BUTTON_8;

		m_enumInputStateMap["RELEASE"] = Rendering::EInputState::RELEASE;
		m_enumInputStateMap["PRESS"] = Rendering::EInputState::PRESS;
		m_enumInputStateMap["REPEAT"] = Rendering::EInputState::REPEAT;
	}

	Rendering::EInputGamepadAxis LuaInputEnum::GetInputGamepadAxis(std::string inputGamepadAxisString)
	{
		const std::unordered_map<std::string, Rendering::EInputGamepadAxis>::iterator it = m_enumInputGamepadAxisMap.find(inputGamepadAxisString);

		if (it == m_enumInputGamepadAxisMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in InputGamepadAxis enum.", inputGamepadAxisString.c_str());
			return Rendering::EInputGamepadAxis::LEFT_TRIGGER;
		}

		return m_enumInputGamepadAxisMap[inputGamepadAxisString];
	}

	Rendering::EInputGamepadButton LuaInputEnum::GetInputGamepadButton(std::string inputGamepadButtonString)
	{
		const std::unordered_map<std::string, Rendering::EInputGamepadButton>::iterator it = m_enumInputGamepadButtonMap.find(inputGamepadButtonString);

		if (it == m_enumInputGamepadButtonMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in InputGamepadButton enum.", inputGamepadButtonString.c_str());
			return Rendering::EInputGamepadButton::A;
		}

		return m_enumInputGamepadButtonMap[inputGamepadButtonString];
	}

	Rendering::EInputKeyboard LuaInputEnum::GetInputKeyboard(std::string inputKeyboardString)
	{
		const std::unordered_map<std::string, Rendering::EInputKeyboard>::iterator it = m_enumInputKeyboardMap.find(inputKeyboardString);

		if (it == m_enumInputKeyboardMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in InputKeyboard enum.", inputKeyboardString.c_str());
			return Rendering::EInputKeyboard::A;
		}

		return m_enumInputKeyboardMap[inputKeyboardString];
	}

	Rendering::EInputMouse LuaInputEnum::GetInputMouse(std::string inputMouseString)
	{
		const std::unordered_map<std::string, Rendering::EInputMouse>::iterator it = m_enumInputMouseMap.find(inputMouseString);

		if (it == m_enumInputMouseMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in InputMouse enum.", inputMouseString.c_str());
			return Rendering::EInputMouse::BUTTON_LEFT;
		}

		return m_enumInputMouseMap[inputMouseString];
	}

	Rendering::EInputState LuaInputEnum::GetInputState(std::string inputStateString)
	{
		const std::unordered_map<std::string, Rendering::EInputState>::iterator it = m_enumInputStateMap.find(inputStateString);

		if (it == m_enumInputStateMap.end())
		{
			LOG(SOURCE_INFO, Core::EVerbosity::LOG_WARNING, "%s dosen't exist in InputState enum.", inputStateString.c_str());
			return Rendering::EInputState::PRESS;
		}

		return m_enumInputStateMap[inputStateString];
	}

}