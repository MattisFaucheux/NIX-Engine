#pragma once
#include <functional>

#include "API/DLLExportRendering.h"

#include <map>
#include <vector>

#include "EInputKeyboard.h"
#include "EInputMouse.h"
#include "EInputGamepadButton.h"
#include "EInputGamepadAxis.h"
#include "EInputState.h"


struct GLFWwindow;

namespace NIX::Rendering
{
	class Window;

	struct CursorPos
	{
		CursorPos() = default;
		
		double x;
		double y;
	};
	
	class InputManager
	{
	public:
		RENDERING_LIBRARY_API InputManager();
		
		RENDERING_LIBRARY_API static InputManager* GetSingleton();
		
		RENDERING_LIBRARY_API [[nodiscard]] EInputState GetInputState(EInputKeyboard key);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputReleased(EInputKeyboard key);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputPressed(EInputKeyboard key);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputRepeated(EInputKeyboard key);

		RENDERING_LIBRARY_API [[nodiscard]] EInputState GetInputState(EInputMouse mouseButton);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputReleased(EInputMouse mouseButton);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputRepeated(EInputMouse mouseButton);
		//TO DO: Replace by vec2
		RENDERING_LIBRARY_API [[nodiscard]] CursorPos GetCursorPos();
		RENDERING_LIBRARY_API [[nodiscard]] double GetCursorPosX();
		RENDERING_LIBRARY_API [[nodiscard]] double GetCursorPosY();

		RENDERING_LIBRARY_API [[nodiscard]] EInputState GetInputState(int jid, EInputGamepadButton button);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputReleased(int jid, EInputGamepadButton button);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputRepeated(int jid, EInputGamepadButton button);
		RENDERING_LIBRARY_API [[nodiscard]] float GetInputAxis(int jid, EInputGamepadAxis axis);

		RENDERING_LIBRARY_API [[nodiscard]] EInputState GetInputState(std::string inputName);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputReleased(std::string inputName);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputPressed(std::string inputName);
		RENDERING_LIBRARY_API [[nodiscard]] bool IsInputRepeated(std::string inputName);
		RENDERING_LIBRARY_API [[nodiscard]] float GetInputAxis(std::string inputName);

		RENDERING_LIBRARY_API bool AddInputBind(std::string& inputName, EInputKeyboard input);
		RENDERING_LIBRARY_API bool AddInputBind(std::string& inputName, EInputMouse input);
		RENDERING_LIBRARY_API bool AddInputBind(std::string& inputName, int jid, EInputGamepadButton input);
		RENDERING_LIBRARY_API bool AddInputBind(std::string& inputName, int jid, EInputGamepadAxis input);

		RENDERING_LIBRARY_API bool RemoveInputBind(std::string inputName);

		template<class T>
		void BindMousePosCallback(T* t, void(T::* method)(double, double))
		{
			m_callbackMousePosFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		void BindMouseScrollCallback(T* t, void(T::* method)(double, double))
		{
			m_callbackMouseScrollFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		void BindJoystickConnectCallback(T* t, void(T::* method)(int, bool))
		{
			m_callbackJoystickConnectFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		void BindDropCallback(T* t, void(T::* method)(int, const char**))
		{
			m_callbackDropFunction = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		void BindKeyCallback(EInputKeyboard key, T* t, void(T::* method)(EInputKeyboard, EInputState))
		{
			m_callbackKeysFunction[key] = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		void BindMouseButtonCallback(EInputMouse button, T* t, void(T::* method)(EInputMouse, EInputState))
		{
			m_callbackMouseButtonsFunction[button] = std::bind(method, t, std::placeholders::_1, std::placeholders::_2);
		}

		RENDERING_LIBRARY_API void UnbindMousePosCallback();
		RENDERING_LIBRARY_API void UnbindMouseScrollCallback();
		RENDERING_LIBRARY_API void UnbindJoystickConnectCallback();
		RENDERING_LIBRARY_API void UnbindDropCallback();
		RENDERING_LIBRARY_API void UnbindKeyCallback(EInputKeyboard key);
		RENDERING_LIBRARY_API void UnbindMouseButtonCallback(EInputMouse button);

	private:
		inline static InputManager* m_singleton = nullptr;

		
		[[nodiscard]] bool CheckIfStringAlreadyBind(std::string& inputName);
		
		void SetKeyState(EInputKeyboard key, EInputState inputState);
		void SetMouseButtonState(EInputMouse button, EInputState buttonState);

		std::map<EInputKeyboard, EInputState> m_keys;
		std::map<EInputMouse, EInputState> m_mouseButtons;
		
		CursorPos m_cursorPos = {0,0};
		CursorPos m_scrollPos = { 0, 0 };

		std::function<void(double, double)> m_callbackMousePosFunction;
		std::function<void(int, bool)> m_callbackJoystickConnectFunction;
		std::function<void(int, const char**)> m_callbackDropFunction;
		std::map<EInputKeyboard, std::function<void(EInputKeyboard, EInputState)>> m_callbackKeysFunction;
		std::map<EInputMouse, std::function<void(EInputMouse, EInputState)>> m_callbackMouseButtonsFunction;
		
		std::map<std::string, EInputKeyboard> m_keyBinds;
		std::map<std::string, EInputMouse> m_mouseButtonsBinds;
		std::function<void(double, double)> m_callbackMouseScrollFunction;
		std::map<std::string, std::pair<int, EInputGamepadButton>> m_gamepadButtonsBinds;
		std::map<std::string, std::pair<int, EInputGamepadAxis>> m_gamepadAxisBinds;
		
	public:
		// Must be called before any KeyInput instances will work
		RENDERING_LIBRARY_API void SetupInputsCallback(Window& window);
	private:
		
		static void KeyboardCallback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/);
		static void CursorPosCallback(GLFWwindow* /*window*/, double xpos, double ypos);
		static void MouseScrollCallback(GLFWwindow* /*window*/, double xoffset, double yoffset);
		static void MouseButtonCallback(GLFWwindow* /*window*/, int button, int action, int /*mods*/);
		static void JoystickCallback(int jid, int event);
		static void DropCallback(GLFWwindow* /*window*/, int count, const char** paths);

	};
}
