#ifndef LUATYPES_HPP
#define LUATYPES_HPP

#include "lua.hpp"

namespace entityx
{
    namespace lua
    {
        /*************
        *  GENERIC  *
        *************/
        template<typename T>
        inline void LuaToC(lua_State* L, T& ref);

        template<typename T>
        inline void CToLua(lua_State* L, T& ref);
        
        /************
        *  DOUBLE  *
        ************/
        template<>
        inline void LuaToC<double>(lua_State* L, double& ref)
        {
            ref = lua_tonumber(L, -1);
        }

        template<>
        inline void CToLua(lua_State* L, double& ref)
        {
            lua_pushnumber(L, ref);
        }

        /***********
        *  FLOAT  *
        ***********/
        template<>
        inline void LuaToC<float>(lua_State* L, float& ref)
        {
            ref = static_cast<float>(lua_tonumber(L, -1));
        }

        template<>
        inline void CToLua<float>(lua_State* L, float& ref)
        {
            lua_pushnumber(L, ref);
        }

        /*************
        *  INTEGER  *
        *************/
        template<>
        inline void LuaToC<int>(lua_State* L, int& ref)
        {
            ref = lua_tointeger(L, -1);
        }

        template<>
        inline void CToLua<int>(lua_State* L, int& ref)
        {
            lua_pushnumber(L, ref);
        }

        /*************
        *  BOOLEAN  *
        *************/
        template<>
        inline void LuaToC<bool>(lua_State* L, bool& ref)
        {
            ref = lua_toboolean(L, -1);
        }

        template<>
        inline void CToLua<bool>(lua_State* L, bool& ref)
        {
            lua_pushboolean(L, ref);
        }

        /************
        *  STRING  *
        ************/
        template<>
        inline void LuaToC<std::string>(lua_State* L, std::string& ref)
        {
            ref = lua_tostring(L, -1);
        }

        template<>
        inline void CToLua<std::string>(lua_State* L, std::string& ref)
        {
            lua_pushstring(L, ref.c_str());
        }
    }
}

#endif//LUATYPES_HPP
