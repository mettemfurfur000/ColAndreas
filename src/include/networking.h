#ifndef NETWORKING
#define NETWORKING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#define MAXRCVLEN 500
#define PORTNUM 2300

#define MAX_THREADS 8

#ifdef __cplusplus
extern "C"
{
#endif

    // utility function
    void ErrorExit(LPCTSTR lpszFunction);

    // returns sockaddr, pointing to server machine (used only on client machine)
    struct sockaddr_in make_sockaddr(int port_number, uint32_t inaddr);

    // acts as recv, just adds null terminator at the end
    int recvstr(int socket, char *dest, const size_t maxlen, int flags);

    int init_client(int *socket_dest, struct sockaddr_in *servaddr_dest);
    int init_server(int *socket_dest, struct sockaddr_in *servaddr_dest);

    void finish_networking();

#ifdef __cplusplus
}
#endif

#endif