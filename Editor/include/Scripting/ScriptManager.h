#pragma once
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "LuaBridge/LuaBridge.h"

#include <string>
#include <iostream>

using namespace luabridge;

namespace NIX::Editor
{

	class GameObject;

	class ScriptManager
	{
	public:

		ScriptManager();

		static ScriptManager* GetSingleton();

		lua_State* CreateNewState(std::string_view filePath);

		template<typename... Args>
		void CallFunc(lua_State* l, const char* funcName ,Args... args)
		{
			LuaRef func = getGlobal(l, funcName);
			if (!func.isNil() && func.isFunction())
			{
				func(args...);
			}
		}



		template<class DataType, typename... Args>
		void AddFunction(const char* funcName, DataType(&func)(Args ...))
		{
			getGlobalNamespace(m_currentL)
				.addFunction(funcName, func);
		};

		template<class DataType, typename... Args>
		void AddFunctionInNamespace(const char* namespaceName, const char* funcName, DataType(&func)(Args ...))
		{
			getGlobalNamespace(m_currentL)
				.beginNamespace(namespaceName)
				.addFunction(funcName, func)
				.endNamespace();
		};



		template<class ClassType, typename ...Args>
		void AddClass(const char* className)
		{
			getGlobalNamespace(m_currentL)
				.beginClass<ClassType>(className)
				.addConstructor<void(*)(Args...)>()
				.endClass();
		};

		template<class ClassType, class DataType, class DataType2>
		void AddClassProperty(const char* className, const char* propertyName, DataType ClassType::* get, DataType2 ClassType::* set)
		{
			getGlobalNamespace(m_currentL)
				.beginClass<ClassType>(className)
				.addProperty(propertyName, get, set)
				.endClass();
		};

		template<class ClassType, class DataType>
		void AddClassFunction(const char* className, const char* funcName, DataType ClassType::* func)
		{
			getGlobalNamespace(m_currentL)
				.beginClass<ClassType>(className)
				.addFunction(funcName, func)
				.endClass();
		};

		template<class ClassType, class DataType>
		void AddClassData(const char* className, const char* dataName, DataType ClassType::* data, bool isWritable)
		{
			getGlobalNamespace(m_currentL)
				.beginClass<ClassType>(className)
				.addData(dataName, data, isWritable)
				.endClass();
		}



		template<class ClassType, class BaseClass, typename ...Args>
		void AddDerivedClass(const char* className)
		{
			getGlobalNamespace(m_currentL)
				.deriveClass<ClassType, BaseClass>(className)
				.addConstructor<void(*)(Args...)>()
				.endClass();
		};

		template<class ClassType, class BaseClass, class DataType, class DataType2>
		void AddDerivedClassProperty(const char* className, const char* propertyName, DataType ClassType::* get, DataType2 ClassType::* set)
		{
			getGlobalNamespace(m_currentL)
				.deriveClass<ClassType, BaseClass>(className)
				.addProperty(propertyName, get, set)
				.endClass();
		};

		template<class ClassType, class BaseClass, class DataType>
		void AddDerivedClassFunction(const char* className, const char* funcName, DataType ClassType::* func)
		{
			getGlobalNamespace(m_currentL)
				.deriveClass<ClassType, BaseClass>(className)
				.addFunction(funcName, func)
				.endClass();
		};

		template<class ClassType, class BaseClass, class DataType>
		void AddDerivedClassData(const char* className, const char* dataName, DataType ClassType::* data, bool isWritable)
		{
			getGlobalNamespace(m_currentL)
				.deriveClass<ClassType, BaseClass>(className)
				.addData(dataName, data, isWritable)
				.endClass();
		}

	private:
		inline static ScriptManager* m_instance = nullptr;

		lua_State* m_currentL = nullptr;
	};


}
