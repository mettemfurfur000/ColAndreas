#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#include "../binn-3.0/src/binn.h"

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vector4;

#define COLANDREASPIPE TEXT("\\\\.\\pipe\\colandreaspipe")
#define BUFFER_SIZE 1024

// keys for a binn maps
#define CMD_TYPE 0
#define ROTATION 1
#define DISTANCE 2
#define MODEL_ID 3
#define RETURN 9

#define VECTOR_OFFSET 10 // allows to stack multiple vectors by adding 10 to the key

#define VECTOR_X 21
#define VECTOR_Y 22
#define VECTOR_Z 23
#define VECTOR_W 24

#define ROTATION_X 25
#define ROTATION_Y 26
#define ROTATION_Z 27

// various types of commands server can receive

#define CMD_RAYCAST 1
#define CMD_RAYCAST_EXTRA 2
#define CMD_EXIT 200

#endif