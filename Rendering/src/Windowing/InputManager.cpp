#include "pch.h"

#include "Windowing/InputManager.h"
#include "Windowing/Window.h"

#include "ImGUI/imgui_impl_glfw.h"
#include "Log/Logger.h"

namespace NIX::Rendering
{
	InputManager::InputManager()
	{
		if (m_singleton != nullptr)
		{
			LOG(SOURCE_INFO,Core::EVerbosity::LOG_ERROR,  "Tried to create a second input manager");
		}
		
		m_singleton = this;
	}

	InputManager* InputManager::GetSingleton()
	{
		if (m_singleton == nullptr)
		{
			ASSERT(SOURCE_INFO, "Input manager singleton is null");
		}

		return m_singleton;
	}

	EInputState InputManager::GetInputState(EInputKeyboard key)
	{
		EInputState inputState = EInputState::RELEASE;

		const std::map<EInputKeyboard, EInputState>::iterator it = m_keys.find(key);
		if (it != m_keys.end())
		{
			inputState = m_keys[key];
		}
		else
		{
			m_keys[key] = inputState;
		}

		return inputState;
	}

	bool InputManager::IsInputReleased(EInputKeyboard key)
	{
		return (GetInputState(key) == EInputState::RELEASE);
	}

	bool InputManager::IsInputPressed(EInputKeyboard key)
	{
		return (GetInputState(key) == EInputState::PRESS);
	}

	bool InputManager::IsInputRepeated(EInputKeyboard key)
	{
		return (GetInputState(key) == EInputState::REPEAT);
	}


	EInputState InputManager::GetInputState(EInputMouse mouseButton)
	{
		EInputState inputState = EInputState::RELEASE;

		const std::map<EInputMouse, EInputState>::iterator it = m_mouseButtons.find(mouseButton);
		if (it != m_mouseButtons.end())
		{
			inputState = m_mouseButtons[mouseButton];
		}
		else
		{
			m_mouseButtons[mouseButton] = inputState;
		}

		return inputState;
	}

	bool InputManager::IsInputReleased(EInputMouse mouseButton)
	{
		return (GetInputState(mouseButton) == EInputState::RELEASE);
	}

	bool InputManager::IsInputRepeated(EInputMouse mouseButton)
	{
		return (GetInputState(mouseButton) == EInputState::REPEAT);
	}

	//TO DO: Replace by vec2
	CursorPos InputManager::GetCursorPos()
	{
		return m_cursorPos;
	}

	double InputManager::GetCursorPosX()
	{
		return m_cursorPos.x;
	}

	double InputManager::GetCursorPosY()
	{
		return m_cursorPos.y;
	}

	EInputState InputManager::GetInputState(int jid, EInputGamepadButton button)
	{
		EInputState inputState = EInputState::RELEASE;
		GLFWgamepadstate state;

		if (glfwGetGamepadState(jid, &state))
		{
			if (state.buttons[(int)button])
			{
				inputState = EInputState::REPEAT;
			}
		}

		return inputState;
	}

	bool InputManager::IsInputReleased(int jid, EInputGamepadButton button)
	{
		return (GetInputState(jid, button) == EInputState::RELEASE);
	}

	bool InputManager::IsInputRepeated(int jid, EInputGamepadButton button)
	{
		return (GetInputState(jid, button) == EInputState::REPEAT);
	}

	float InputManager::GetInputAxis(int jid, EInputGamepadAxis axis)
	{
		GLFWgamepadstate state;

		if (glfwGetGamepadState(jid, &state))
		{
			return state.axes[(int)axis];
		}

		return 0.f;
	}

	EInputState InputManager::GetInputState(std::string inputName)
	{
		const std::map<std::string, EInputKeyboard>::iterator keyIt = m_keyBinds.find(inputName);
		if (keyIt != m_keyBinds.end())
		{
			return GetInputState(m_keyBinds[inputName]);
		}

		const std::map<std::string, EInputMouse>::iterator mButtonIt = m_mouseButtonsBinds.find(inputName);
		if (mButtonIt != m_mouseButtonsBinds.end())
		{
			return GetInputState(m_mouseButtonsBinds[inputName]);
		}

		const std::map<std::string, std::pair<int, EInputGamepadButton>>::iterator gButtonIt = m_gamepadButtonsBinds.find(inputName);
		if (gButtonIt != m_gamepadButtonsBinds.end())
		{
			std::pair<int, EInputGamepadButton> result = m_gamepadButtonsBinds[inputName];
			return GetInputState(result.first, result.second);
		}

		return EInputState::RELEASE;
	}

	bool InputManager::IsInputReleased(std::string inputName)
	{
		return (GetInputState(inputName) == EInputState::RELEASE);
	}

	bool InputManager::IsInputPressed(std::string inputName)
	{
		return(GetInputState(inputName) == EInputState::PRESS);
	}

	bool InputManager::IsInputRepeated(std::string inputName)
	{
		return (GetInputState(inputName) == EInputState::REPEAT);
	}

	float InputManager::GetInputAxis(std::string inputName)
	{
		const std::map<std::string, std::pair<int, EInputGamepadAxis>>::iterator it = m_gamepadAxisBinds.find(inputName);
		if (it != m_gamepadAxisBinds.end())
		{
			std::pair<int, EInputGamepadAxis> result = m_gamepadAxisBinds[inputName];
			return GetInputAxis(result.first, result.second);
		}

		return 0.f;
	}

	bool InputManager::AddInputBind(std::string& inputName, EInputKeyboard input)
	{
		if (CheckIfStringAlreadyBind(inputName))
		{
			return false;
		}

		m_keyBinds[inputName] = input;
		return true;
	}

	bool InputManager::AddInputBind(std::string& inputName, EInputMouse input)
	{
		if (CheckIfStringAlreadyBind(inputName))
		{
			return false;
		}

		m_mouseButtonsBinds[inputName] = input;
		return true;
	}

	bool InputManager::AddInputBind(std::string& inputName, int jid, EInputGamepadButton input)
	{
		if (CheckIfStringAlreadyBind(inputName))
		{
			return false;
		}

		m_gamepadButtonsBinds[inputName] = std::pair<int, EInputGamepadButton>(jid, input);
		return true;
	}

	bool InputManager::AddInputBind(std::string& inputName, int jid, EInputGamepadAxis input)
	{
		if (CheckIfStringAlreadyBind(inputName))
		{
			return false;
		}

		m_gamepadAxisBinds[inputName] = std::pair<int, EInputGamepadAxis>(jid, input);
		return true;
	}

	bool InputManager::RemoveInputBind(std::string inputName)
	{
		return( m_keyBinds.erase(inputName) != 0 ||
				m_mouseButtonsBinds.erase(inputName) != 0 ||
				m_gamepadButtonsBinds.erase(inputName) != 0 ||
				m_gamepadAxisBinds.erase(inputName) != 0);

	}

	void InputManager::UnbindMousePosCallback()
	{
		m_callbackMousePosFunction = nullptr;
	}

	void InputManager::UnbindMouseScrollCallback()
	{
		m_callbackMouseScrollFunction = nullptr;
	}

	void InputManager::UnbindJoystickConnectCallback()
	{
		m_callbackJoystickConnectFunction = nullptr;
	}

	void InputManager::UnbindDropCallback()
	{
		m_callbackDropFunction = nullptr;
	}

	void InputManager::UnbindKeyCallback(EInputKeyboard key)
	{
		m_callbackKeysFunction.erase(key);
	}

	void InputManager::UnbindMouseButtonCallback(EInputMouse button)
	{
		m_callbackMouseButtonsFunction.erase(button);
	}

	bool InputManager::CheckIfStringAlreadyBind(std::string& inputName)
	{

		return(m_keyBinds.find(inputName) != m_keyBinds.end() ||
			m_mouseButtonsBinds.find(inputName) != m_mouseButtonsBinds.end() ||
			m_gamepadButtonsBinds.find(inputName) != m_gamepadButtonsBinds.end() ||
			m_gamepadAxisBinds.find(inputName) != m_gamepadAxisBinds.end());

	}

	void InputManager::SetKeyState(EInputKeyboard key, EInputState inputState)
	{
		m_keys[key] = inputState;
	}

	void InputManager::SetMouseButtonState(EInputMouse button, EInputState buttonState)
	{
		m_mouseButtons[button] = buttonState;
	}

	void InputManager::SetupInputsCallback(Window& window)
	{
		GLFWwindow* win = window.GetWindow();

		glfwSetKeyCallback(win, InputManager::KeyboardCallback);
		glfwSetCursorPosCallback(win, InputManager::CursorPosCallback);
		glfwSetScrollCallback(win, InputManager::MouseScrollCallback);
		glfwSetMouseButtonCallback(win, InputManager::MouseButtonCallback);
		glfwSetJoystickCallback(InputManager::JoystickCallback);
		glfwSetDropCallback(win, InputManager::DropCallback);
	}

	void InputManager::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		const EInputKeyboard input = static_cast<EInputKeyboard>(key);
		const EInputState state = static_cast<EInputState>(action);

		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		m_singleton->SetKeyState(input, state);
				
		const auto it = m_singleton->m_callbackKeysFunction.find(input);
		if (it != m_singleton->m_callbackKeysFunction.end())
		{
			m_singleton->m_callbackKeysFunction[input](input, state);
		}
	}

	void InputManager::MouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
	{
		const EInputMouse input = static_cast<EInputMouse>(button);
		const EInputState state = action == GLFW_PRESS ? EInputState::REPEAT : EInputState::RELEASE;

		m_singleton->SetMouseButtonState(input, state);

		const auto it = m_singleton->m_callbackMouseButtonsFunction.find(input);
		if (it != m_singleton->m_callbackMouseButtonsFunction.end())
		{
			m_singleton->m_callbackMouseButtonsFunction[input](input, state);
		}
	}

	void InputManager::CursorPosCallback(GLFWwindow* /*window*/, double xpos, double ypos)
	{
		//TO DO: Replace by vec2
		m_singleton->m_cursorPos = { xpos, ypos };

		if (m_singleton->m_callbackMousePosFunction)
		{
			m_singleton->m_callbackMousePosFunction(xpos, ypos);
		}
	}

	void InputManager::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{		
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

		if (m_singleton->m_callbackMouseScrollFunction)
		{
			m_singleton->m_callbackMouseScrollFunction(xoffset, yoffset);
		}
	}

	void InputManager::JoystickCallback(int jid, int event)
	{
		if (m_singleton->m_callbackJoystickConnectFunction)
		{
			m_singleton->m_callbackJoystickConnectFunction(jid, event == GLFW_CONNECTED);
		}
	}

	void InputManager::DropCallback(GLFWwindow*, int count, const char** paths)
	{
		if (m_singleton->m_callbackDropFunction)
		{
			m_singleton->m_callbackDropFunction(count, paths);
		}
	}
}
