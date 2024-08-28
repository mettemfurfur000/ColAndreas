#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lua.h>

__declspec(dllexport) int luaopen_ColAndreas(lua_State *L);

__declspec(dllexport) int lua_connect(lua_State *L);
__declspec(dllexport) int lua_exit(lua_State *L);
__declspec(dllexport) int lua_raycast_line(lua_State *L);

#endif