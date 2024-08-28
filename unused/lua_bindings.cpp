#include "include/lua_bindings.h"
#include "include/DynamicWorld.h"
#include "include/renderware.h"

bool colInit = false;
bool colDataLoaded = false;
// cell nullAddress = NULL;

// require "ColAndreas"

ColAndreasWorld *collisionWorld;

extern "C" __declspec(dllexport) int luaopen_ColAndreas(lua_State *L)
{
    luaL_Reg lua_lib[] = {
        {"Init", lua_Init},
        {"RayCastLine", lua_RayCastLine},
        {"RemoveBuilding", lua_RemoveBuilding},
        {NULL, NULL}

    };
    // luaL_newlib(L, lua_lib);

    luaL_checkversion_(L, 503, (sizeof(lua_Integer) * 16 + sizeof(lua_Number)));
    lua_createtable(L, 0, sizeof(lua_lib) / sizeof((lua_lib)[0]) - 1);
    luaL_setfuncs(L, lua_lib, 0);
    // luaL_register(L, "ColAndreas", lua_lib);
    return 1;
}

extern "C" __declspec(dllexport) int lua_RemoveBuilding(lua_State *L)
{
    if (!colInit)
    {
        removeBuildingData tmp;
        tmp.r_Model = (int16_t)lua_tonumber(L, 1);
        tmp.r_X = lua_tonumber(L, 2);
        tmp.r_Y = lua_tonumber(L, 3);
        tmp.r_Z = lua_tonumber(L, 4);
        tmp.r_Radius = lua_tonumber(L, 5);

        collisionWorld->removedManager->addBuilding(tmp);
        lua_pushboolean(L, 1);
        return 1;
    }
    else
    {
        // logprintf("ERROR: RemoveBuilding : Map has already been initialized. Use this before Init.");
    }
    return 0;
}

extern "C" __declspec(dllexport) int lua_Init(lua_State *L)
{
    collisionWorld = new ColAndreasWorld();

    if (colDataLoaded)
        return 0;

    colDataLoaded = collisionWorld->loadCollisionData(lua_tostring(L, 1));

    if (colInit)
        return 0;

    collisionWorld->colandreasInitMap();
    colInit = true;

    return 0;
}

btVector3 util_get_vector3(lua_State *L, int index)
{
    btVector3 vector;
    vector.setX(lua_tonumber(L, index) + 0.00001);
    vector.setY(lua_tonumber(L, index + 1) + 0.00001);
    vector.setZ(lua_tonumber(L, index + 2) + 0.00001);
    return vector;
}

extern "C" __declspec(dllexport) int lua_RayCastLine(lua_State *L)
{
    btVector3 Start = util_get_vector3(L, 1);
    btVector3 End = util_get_vector3(L, 4);

    btVector3 Result;
    int32_t Model = 0;
    if (collisionWorld->performRayTest(Start, End, Result, Model))
    {
        lua_pushnumber(L, Result.getX());
        lua_pushnumber(L, Result.getY());
        lua_pushnumber(L, Result.getZ());
        lua_pushnumber(L, Model);
        return 4;
    }
    return 0;
}

extern "C" __declspec(dllexport) int lua_RayCastLineExtraID(lua_State *L)
{
    btVector3 Start = util_get_vector3(L, 1);
    btVector3 End = util_get_vector3(L, 4);
    int32_t Model = lua_tointeger(L, 7);
    uint16_t Data = 0;
    btVector3 Result;

    if (collisionWorld->performRayTestExtraID(Start, End, Result, Model, Data))
    {
        lua_pushnumber(L, Result.getX());
        lua_pushnumber(L, Result.getY());
        lua_pushnumber(L, Result.getZ());
        lua_pushnumber(L, Model);
        return 4;
    }
    return 0;
}

extern "C" __declspec(dllexport) int lua_RayCastLineAngle(lua_State *L)
{
    btVector3 Start = util_get_vector3(L, 1);
    float angle = lua_tonumber(L, 4);
    float distance = lua_tonumber(L, 5);

    btVector3 End;
    End.setX(Start.getX() + (distance * cos(angle)));
    End.setY(Start.getY() + (distance * sin(angle)));
    End.setZ(Start.getZ());

    btVector3 Result;
    int32_t Model = 0;
    if (collisionWorld->performRayTest(Start, End, Result, Model))
    {
        lua_pushnumber(L, Result.getX());
        lua_pushnumber(L, Result.getY());
        lua_pushnumber(L, Result.getZ());
        lua_pushnumber(L, Model);
        return 4;
    }
    return 0;
}

// __declspec(dllexport) int lua_ContactTest(lua_State *L)
// {
//     btVector3 Position = util_get_vector3(L, 1);
//     float Radius = lua_tonumber(L, 4);

//     int32_t Model = 0;
//     if (collisionWorld->performContactTest(Position, Radius, Model))
//     {
//         lua_pushboolean(L, true);
//         lua_pushnumber(L, Model);
//         return 2;
//     }
//     lua_pushboolean(L, false);
//     return 1;
// }
