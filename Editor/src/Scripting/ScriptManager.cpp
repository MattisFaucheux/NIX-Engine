#include "pch.h"

#include "Scripting/ScriptManager.h"
#include "Log/Logger.h"

#include "GameObject.h"

#include "Scripting/LuaLog.h"
#include "Scripting/LuaVec3.h"
#include "Scripting/LuaVec4.h"
#include "Scripting/LuaQuaternion.h"
#include "Scripting/LuaInputManager.h"
#include "Scripting/LuaSceneManager.h"
#include "Scripting/LuaScene.h"
#include "Scripting/LuaRaycast.h"


namespace NIX::Editor
{
    ScriptManager::ScriptManager()
    {
        if (m_instance != nullptr)
        {
            return;
        }

        m_instance = this;
    }

    ScriptManager* ScriptManager::GetSingleton()
    {
        return m_instance;
    }

    lua_State* ScriptManager::CreateNewState(std::string_view filePath)
    {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);

        m_currentL = L;

        LuaLog::LuaInit();
        LuaVec3::LuaInit();
        LuaVec4::LuaInit();
        LuaQuaternion::LuaInit();
        LuaInputManager::LuaInit();
        LuaSceneManager::LuaInit();
        LuaScene::LuaInit();
        GameObject::LuaInit();
        LuaRaycast::LuaInit();

        m_currentL = nullptr;

        if (luaL_loadfile(L, filePath.data()) || lua_pcall(L, 0, 0, 0))
        {
            LOG(SOURCE_INFO, Core::EVerbosity::LOG_ERROR, "Can't load : '%s'", filePath.data());
            return nullptr;
        }

        return L;
    }
}