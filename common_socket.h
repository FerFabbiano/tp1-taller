#ifndef SOCKET_H
#define SOCKET_H

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define ERROR 1
#define CLIENT 0
#define SERVER 1

typedef struct addrinfo addrinfo;

typedef struct socket_t{
    int socket;
}socket_t;  

int socket_create(socket_t *self);

int socket_destroy(socket_t *self);

int socket_bind_and_listen(socket_t *self, const char *service);

int socket_accept(socket_t *self, socket_t *accepted_socket);

int socket_connect(socket_t *self, const char *host_name, const char *service);

int socket_send(socket_t *self, const char *buffer, size_t length);

int socket_receive(socket_t *self, char *buffer, size_t buf_length);

void socket_shutdown(socket_t *self, int channel);

void socket_set_hints(struct addrinfo *hints, int tipo);

#endif
