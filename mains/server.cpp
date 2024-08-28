#include "../src/include/networking.h"
#include "../src/include/DynamicWorld.h"
#include "../src/include/renderware.h"

// #include "../src/include/plugin_interface.h"

#include "interface.h"

DWORD WINAPI InstanceThread(LPVOID);
VOID GetAnswerToRequest(byte *input, byte *output, size_t got_bytes, size_t *length);

// cell nullAddress = NULL;

// require "ColAndreas"

int server_connections_handler(int socket, struct sockaddr_in *servaddr, DWORD (*client_handler)(void *));
unsigned long client_handler(void *param);

ColAndreasWorld *collisionWorld;
int main(int argc, char *argv[])
{
    int mysocket;
    struct sockaddr_in servaddr;

    collisionWorld = new ColAndreasWorld();

    collisionWorld->loadCollisionData("world.cadb");

    collisionWorld->colandreasInitMap();

    int ret = init_server(&mysocket, &servaddr);

    if (ret == -1)
    {
        // printf("Error in init_server\n");
        ErrorExit(TEXT("GetProcessId"));
        return EXIT_FAILURE;
    }

    server_connections_handler(mysocket, &servaddr, &client_handler);

    close(mysocket);

    finish_networking();

    return EXIT_SUCCESS;
}

unsigned long client_handler(void *param)
{
    byte input[BUFFER_SIZE] = {};
    size_t got_bytes;

    int consocket = *(int *)param;

    byte type = -1;
    btVector3 Start;
    btVector3 End;
    float angle;
    float distance;

    btVector3 Result;
    int32_t Model = 0;

    bool do_loop = true;

    while (do_loop)
    {
        got_bytes = recv(consocket, (char *)input, BUFFER_SIZE, 0);
        type = binn_map_int8(input, CMD_TYPE);

        binn *out_map = binn_map();

        switch (type)
        {
        case CMD_EXIT:
            printf("Client disconnected\n");
            do_loop = false;
            break;
        case CMD_RAYCAST:
            Start.setX(binn_map_float(input, VECTOR_X));
            Start.setY(binn_map_float(input, VECTOR_Y));
            Start.setZ(binn_map_float(input, VECTOR_Z));

            // printf("Start: %f, %f, %f\n", Start.getX(), Start.getY(), Start.getZ());

            End.setX(binn_map_float(input, VECTOR_X + VECTOR_OFFSET));
            End.setY(binn_map_float(input, VECTOR_Y + VECTOR_OFFSET));
            End.setZ(binn_map_float(input, VECTOR_Z + VECTOR_OFFSET));

            // printf("End: %f, %f, %f\n", End.getX(), End.getY(), End.getZ());
            if (collisionWorld->performRayTest(Start, End, Result, Model))
            {
                // printf("Hit: %f, %f, %f\n", Result.getX(), Result.getY(), Result.getZ());
                // printf("Model: %d\n", Model);
                binn_map_set_float(out_map, VECTOR_X, Result.getX());
                binn_map_set_float(out_map, VECTOR_Y, Result.getY());
                binn_map_set_float(out_map, VECTOR_Z, Result.getZ());
                binn_map_set_int32(out_map, MODEL, Model);
                binn_map_set_int32(out_map, RETURN, 1);
            }
            else
            {
                binn_map_set_int32(out_map, RETURN, 0);
            }
            break;
        default:
            printf("Unknown command\n");
            binn_map_set_int32(out_map, RETURN, -1);
            break;
        }

        if (do_loop == false)
            break; // prevent sending data to disconnected client

        send(consocket, (char *)binn_ptr(out_map), binn_size(out_map), 0);
        binn_free(out_map);
    }

    close(consocket);

    return 0;
}

int server_connections_handler(int socket, struct sockaddr_in *servaddr, DWORD (*hander)(void *))
{
    struct sockaddr_in client;
    socklen_t socksize = sizeof(struct sockaddr_in);
    listen(socket, SOMAXCONN);

    HANDLE thread_handles[MAX_THREADS];
    int thread_count = 0;

    while (1)
    {
        // printf("Waiting for a connection...\n");
        int consocket = accept(socket, (struct sockaddr *)&client, &socksize);
        if (consocket == INVALID_SOCKET)
        {
            // printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }

        // printf("Incoming connection from %s - creating new thread\n", inet_ntoa(client.sin_addr));
#define USE_THREADS 1
#ifdef USE_THREADS
        HANDLE thread = CreateThread(NULL, 0, client_handler, &consocket, 0, NULL);

        if (thread == NULL)
        {
            printf("CreateThread failed: %d\n", GetLastError());
            closesocket(consocket);
        }
        else
        {
            thread_handles[thread_count++] = thread;
            // printf("Thread %d created\n", thread_count);
            if (thread_count >= MAX_THREADS)
            {
                printf("Max threads reached, waiting for threads to finish\n");
                WaitForMultipleObjects(MAX_THREADS, thread_handles, TRUE, INFINITE);
                for (int i = 0; i < MAX_THREADS; i++)
                {
                    CloseHandle(thread_handles[i]);
                }
                thread_count = 0;
            }
        }
#else
        client_handler(&consocket);
#endif
    }

    closesocket(socket);
    return 0;
}