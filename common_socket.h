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

/* Creo el socket. */
int socket_create(socket_t *self);

/* Cierro el socket. */
int socket_destroy(socket_t *self);

/*
 Enlazo el socket a la interfaz y a un puerto determinado. Lo hago a través de bind. 
 Una vez enlazado, queremos usar el socket para escuchar conexiones entrantes. 
 Se van a ir encolando.
 */
int socket_bind_and_listen(socket_t *self, const char *service);

/* 
Acepto uno de los clientes y lo saco de la cola.
*/
int socket_accept(socket_t *self, socket_t *accepted_socket);

/* Función del cliente. Me conecto a un servidor. */
int socket_connect(socket_t *self, const char *host_name, const char *service);

/* Envio mensajes a través del socket. */
int socket_send(socket_t *self, const char *buffer, size_t length);

/* Recibo mensajes a través del socket. */
int socket_receive(socket_t *self, char *buffer, size_t buf_length);

/* Cierro los canales de comunicación del socket.
Tanto de lectura como de escritura. */
void socket_shutdown(socket_t *self, int channel);

/* Seteo las hints, dependiendo se si me solicita el servidor, o
el cliente. */
void socket_set_hints(struct addrinfo *hints, int tipo);

#endif
