#define _POSIX_C_SOURCE 200112L
#include "common_socket.h"

int socket_create(socket_t *self){
    self->socket = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    int s = 0;
    s = setsockopt(self->socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (s == -1) {
      close(self->socket);
      return 1;
   }
    if (self->socket < 0){
        return ERROR;
    }
    return 0;
}

int socket_destroy(socket_t *self){
    close(self->socket);
    return 0;
}

/*
 Enlazo el socket a la interfaz y a un puerto determinado. Lo hago a través de bind. 
 Una vez enlazado, queremos usar el socket para escuchar conexiones entrantes. 
 Se van a ir encolando.
 */
int socket_bind_and_listen(socket_t *self, const char *service){
    addrinfo hints, *results, *aux;
    int status = 0;
    socket_set_hints(&hints, SERVER);
    status = getaddrinfo(0, service, &hints, &results);  
    if (status != 0) { 
        return ERROR;
    }
    for (aux = results; aux != NULL; aux = aux->ai_next) {
        if (bind(self->socket, aux->ai_addr, aux->ai_addrlen) == 0){
            break;               
        }
        close(self->socket);
        socket_create(self);
        if (aux == NULL) {              
            close(self->socket);
            return ERROR;
        }
    }
    freeaddrinfo(results); /* libero lista de direcciones. */
    if (listen(self->socket, 10) == -1){ 
        close(self->socket);
        return ERROR;
    }
    return 0;
}

/* 
Acepto uno de los clientes y lo saco de la cola.
*/

int socket_accept(socket_t *self, socket_t *accepted_socket){
    int acepted = 0;
    acepted = accept(self->socket, NULL, NULL);
    if (acepted != -1){
        accepted_socket->socket = acepted;
    }
    return 0;
}

int socket_connect(socket_t *self, const char *host_name, const char *service){
    addrinfo hints, *result, *aux;
    int skt = 0;
    memset(&hints, 0, sizeof(struct addrinfo));
    socket_set_hints(&hints, CLIENT);   
    skt = getaddrinfo(host_name, service, &hints, &result);      
    if (skt != 0) {
        close(self->socket);
        return ERROR;
    }
    for (aux = result; aux != NULL; aux = aux->ai_next) {
        if (connect(self->socket, aux->ai_addr, aux->ai_addrlen) == 0){      
            break; 
        }  
        close(self->socket);
        socket_create(self);        
    }
    if (aux == NULL){              
        return ERROR;
    }
    freeaddrinfo(result); 
    return 0;          
}

int socket_send(socket_t *self, const char *buffer, size_t buf_length){
        ssize_t size_send, aux;
        size_send = 0;
        int s = self->socket;
        while (size_send < buf_length){
            aux = 0;
            aux = send(s, &buffer[size_send], buf_length - size_send, 0);
            if (aux <= 0){ 
                return ERROR;
            }
            size_send += aux;
        }
        return 0;
}

int socket_receive(socket_t *self, char *buffer, size_t buf_length){
    ssize_t size_rcv, aux;
    bool socket_is_close = false;
    size_rcv = 0;
    int s = self->socket;
    while (socket_is_close == false){
        aux = 0;
        aux = recv(s, &buffer[size_rcv], buf_length - size_rcv - 1, 0);
        if (aux < 0){
            return ERROR;
        }else if (aux == 0){
            socket_is_close = true;
        }else{
            size_rcv += aux;
        }
    }
    return 0;
} 

void socket_shutdown(socket_t *self, int channel){
    shutdown(self->socket, channel);
}

void socket_set_hints(addrinfo *hints, int tipo){
    hints->ai_family = AF_INET;    
    hints->ai_socktype = SOCK_STREAM; 
    hints->ai_protocol = 0;
    switch (tipo){
        case 0: hints->ai_flags = 0;
        case 1: hints->ai_flags = AI_PASSIVE;
    }
}
