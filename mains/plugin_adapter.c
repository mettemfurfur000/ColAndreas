#include "../src/include/networking.h"
// #include "../src/include/DynamicWorld.h"
// #include "../src/include/renderware.h"
#include "interface.h"
#include <stdint.h>
#include <stdbool.h>

#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lua.h>

// ca = require("calua")

int open_socket;
struct sockaddr_in servaddr;
char buffer[BUFFER_SIZE] = {0};

__declspec(dllexport) int lua_connect(lua_State *L)
{
    int ret = init_client(&open_socket, &servaddr);

    if (ret == -1)
    {
        printf("Error in init_client\n");
        ErrorExit(TEXT("GetProcessId"));
        return EXIT_FAILURE;
    }

    return 0;
}

__declspec(dllexport) int lua_exit(lua_State *L)
{
    binn *scratch_map = binn_map();
    binn_map_set_int8(scratch_map, CMD_TYPE, CMD_EXIT);

    send(open_socket, (char *)binn_ptr(scratch_map), binn_size(scratch_map), 0);

    binn_free(scratch_map);
    close(open_socket);

    return 0;
}

__declspec(dllexport) int lua_raycast_line(lua_State *L)
{
    int32_t Model = 0;
    unsigned long length_out;

    binn *scratch_map = binn_map();

    binn_map_set_int8(scratch_map, CMD_TYPE, CMD_RAYCAST);

    binn_map_set_float(scratch_map, VECTOR_X, lua_tonumber(L, 1) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Y, lua_tonumber(L, 2) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Z, lua_tonumber(L, 3) + 0.00001);

    binn_map_set_float(scratch_map, VECTOR_X + VECTOR_OFFSET, lua_tonumber(L, 4) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Y + VECTOR_OFFSET, lua_tonumber(L, 5) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Z + VECTOR_OFFSET, lua_tonumber(L, 6) + 0.00001);

    send(open_socket, (char *)binn_ptr(scratch_map), binn_size(scratch_map), 0);

    binn_free(scratch_map);

    memset(buffer, 0, sizeof(buffer));

    (void)recv(open_socket, buffer, sizeof(buffer), 0);

    int ret = binn_map_int32(buffer, RETURN);
    lua_pushnumber(L, ret);

    if (ret == 1)
    {
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_X));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Y));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Z));

        lua_pushnumber(L, (double)binn_map_int32(buffer, MODEL_ID));
        return 5;
    }

    return 1;
}

__declspec(dllexport) int lua_raycast_line_extra(lua_State *L)
{
    int32_t Model = 0;
    unsigned long length_out;

    binn *scratch_map = binn_map();

    binn_map_set_int8(scratch_map, CMD_TYPE, CMD_RAYCAST_EXTRA);

    binn_map_set_float(scratch_map, VECTOR_X, lua_tonumber(L, 1) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Y, lua_tonumber(L, 2) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Z, lua_tonumber(L, 3) + 0.00001);

    binn_map_set_float(scratch_map, VECTOR_X + VECTOR_OFFSET, lua_tonumber(L, 4) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Y + VECTOR_OFFSET, lua_tonumber(L, 5) + 0.00001);
    binn_map_set_float(scratch_map, VECTOR_Z + VECTOR_OFFSET, lua_tonumber(L, 6) + 0.00001);

    send(open_socket, (char *)binn_ptr(scratch_map), binn_size(scratch_map), 0);

    binn_free(scratch_map);

    memset(buffer, 0, sizeof(buffer));

    (void)recv(open_socket, buffer, sizeof(buffer), 0);

    int ret = binn_map_int32(buffer, RETURN);
    lua_pushnumber(L, ret);

    if (ret == 1)
    {
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_X)); // result collision point
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Y));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Z));

        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_X + VECTOR_OFFSET)); // it says vector, but its a quaternion for rotation of a model that was hit
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Y + VECTOR_OFFSET));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Z + VECTOR_OFFSET));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_W + VECTOR_OFFSET));

        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_X + VECTOR_OFFSET * 2)); // position of the model that was hit
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Y + VECTOR_OFFSET * 2));
        lua_pushnumber(L, (double)binn_map_float(buffer, VECTOR_Z + VECTOR_OFFSET * 2));

        lua_pushnumber(L, (double)binn_map_int32(buffer, MODEL_ID));

        return 12;
    }

    return 1;
}

static const luaL_Reg calua_lib[] = {
    //{"Init", lua_Init},
    {"connect", lua_connect},
    {"raycast_line", lua_raycast_line},
    {"raycast_line_extra", lua_raycast_line_extra},
    {"exit", lua_exit},
    //{"RemoveBuilding", lua_RemoveBuilding},
    {NULL, NULL}

};

__declspec(dllexport) int luaopen_calua_lib(lua_State *L)
{
    // luaL_newlib(L, calua_lib);
    luaL_register(L, "calua_lib", calua_lib);
    return 1;
}

// __declspec(dllexport) int lua_RayCastLineExtraID(lua_State *L)
// {
//     btVector3 Start = util_get_vector3(L, 1);
//     btVector3 End = util_get_vector3(L, 4);
//     int32_t Model = lua_tointeger(L, 7);
//     uint16_t Data = 0;
//     btVector3 Result;

//     if (collisionWorld->performRayTestExtraID(Start, End, Result, Model, Data))
//     {
//         lua_pushnumber(L, Result.getX());
//         lua_pushnumber(L, Result.getY());
//         lua_pushnumber(L, Result.getZ());
//         lua_pushnumber(L, Model);
//         return 4;
//     }
//     return 0;
// }

// __declspec(dllexport) int lua_RayCastLineAngle(lua_State *L)
// {
//     btVector3 Start = util_get_vector3(L, 1);
//     float angle = lua_tonumber(L, 4);
//     float distance = lua_tonumber(L, 5);

//     btVector3 End;
//     End.setX(Start.getX() + (distance * cos(angle)));
//     End.setY(Start.getY() + (distance * sin(angle)));
//     End.setZ(Start.getZ());

//     btVector3 Result;
//     int32_t Model = 0;
//     if (collisionWorld->performRayTest(Start, End, Result, Model))
//     {
//         lua_pushnumber(L, Result.getX());
//         lua_pushnumber(L, Result.getY());
//         lua_pushnumber(L, Result.getZ());
//         lua_pushnumber(L, Model);
//         return 4;
//     }
//     return 0;
// }

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
