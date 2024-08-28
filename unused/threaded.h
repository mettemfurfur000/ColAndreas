
#ifndef THREADED_H
#define THREADED_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct
{
    int socket_fd;
    struct sockaddr_in address;
} client_t;

typedef struct
{
    client_t clients[MAX_CLIENTS];
    int client_count;
    pthread_mutex_t client_mutex;
    atomic_bool running;
} server_t;

typedef struct
{
    server_t *server;
    int client_index;
} client_handler_args_t;

// Server functions
void server_init(server_t *server);
void server_start(server_t *server, int port);
void server_stop(server_t *server);
void *accept_clients(void *arg);

// Client handling functions
void *handle_client(void *arg);
void add_client(server_t *server, int client_socket, struct sockaddr_in client_address);
void remove_client(server_t *server, int client_index);

// Utility functions
void send_message(int socket_fd, const char *message);
int receive_message(int socket_fd, char *buffer, size_t buffer_size);

#endif // THREADED_H
