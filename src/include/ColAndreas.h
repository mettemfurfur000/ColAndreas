// Bet this is it here

#ifndef _INCL_COLANDREAS
#define _INCL_COLANDREAS

#include <BulletDynamics/btBulletDynamicsCommon.h>

#pragma warning(disable : 4005 700 996)
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#else
#include "pthread.h"
#endif
#include "sstream"
#if defined(LINUX) || defined(FREEBSD) || defined(__FreeBSD__) || defined(__OpenBSD__)
#include "stdarg.h"
#endif

#ifndef WIN32
typedef unsigned long DWORD;
#define MAX_PATH 250
#endif

// // AMX
// typedef void (*logprintf_t)(char *format, ...);
// #include "../SDK/amx/amx.h"
// #include "../SDK/plugincommon.h"

#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lua.h>

// Standard includes
#include <vector>

extern bool colInit;
extern bool colDataLoaded;
// extern cell nullAddress;

#endif