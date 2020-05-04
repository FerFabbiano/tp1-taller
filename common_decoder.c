#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common_decoder.h"
#include "common_others.h"

int decoder_get_size_of_array_opt(char *header){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = header[12+i];
    }
    numero = to_little_endian(numero);
    return numero;
}

int decoder_get_size_of_body(char *header){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = header[4+i];
    }
    numero = to_little_endian(numero);
    return numero;
}

data_t decoder_get_ruta(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memset(data.ruta, 0, 32);
    memcpy(data.ruta, &array_opt[pos_array_opt + 8], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_destino(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memset(data.destino, 0, 32);
    memcpy(data.destino, &array_opt[pos_array_opt + 8], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_interfaz(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memset(data.interfaz, 0, 32);
    memcpy(data.interfaz, &array_opt[pos_array_opt + 8], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_metodo(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memset(data.metodo, 0, 32);
    memcpy(data.metodo, &array_opt[pos_array_opt + 8], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_manage_type(int type, char *array_opt, 
size_t pos_array_opt, data_t data){
    switch (type){
        case 1: data = decoder_get_ruta(array_opt, pos_array_opt, data);
        break;
        case 2: data = decoder_get_interfaz(array_opt, pos_array_opt, data);
        break;
        case 3: data = decoder_get_metodo(array_opt, pos_array_opt, data);
        break;
        case 6: data = decoder_get_destino(array_opt, pos_array_opt, data);
        break;
        default: break;
    }
    return data;
}

int decoder_get_size_of_firma(char *array_opt, size_t size_array_opt){
    int length_firma = 0;
    int i = 0;
    while (array_opt[size_array_opt - length_firma] != 9
    && (i < size_array_opt)){
        length_firma += 1;
    }
    if ((length_firma+1) == size_array_opt){
        return 0;
    }else{
        return length_firma+1;
    }
}

int decoder_print_data(char *array_opt, size_t size_array_opt){
    data_t data;
    data.pos_array_opt = 0;
    int parameter_type = 0;
    int length_firma = decoder_get_size_of_firma(array_opt, size_array_opt);
    while (data.pos_array_opt < (size_array_opt - length_firma)){
        parameter_type = array_opt[data.pos_array_opt];
        data = decoder_manage_type(parameter_type, array_opt, 
        data.pos_array_opt, data);
    }
    printf("* Destino: %s\n* Path: %s\n* Interfaz: %s\n* Metodo: %s\n",
    data.destino, data.ruta, data.interfaz, data.metodo);
    if (length_firma == 0){
        printf("\n");
    }
    return 0;
}

body_t decoder_get_parameter(char *body, size_t pos_body, body_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = body[pos_body + i];
    }
    numero = to_little_endian(numero);
    char *parametro = (char*) malloc(numero);
    for (int i = 0; i < numero; i++){
        parametro[i] = body[pos_body+4+i];
    }
    memcpy(data.parametro, parametro, numero);
    data.pos_body += numero + 5;
    free(parametro);
    return data;
}

int decoder_print_body(char *body, size_t size_body){
    body_t data;
    data.pos_body = 0;
    if (size_body == 0){
        return 0;
    }
    printf("* Parametros: \n");
    while (data.pos_body < (size_body)){
        data = decoder_get_parameter(body, data.pos_body, data);
        printf("    * %s\n", data.parametro);
        memset(data.parametro, 0, 32);
    }
    printf("\n");
    return 0;    
}

int decoder_print_id_number(char *header){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = header[8 + i];
    }
    numero = to_little_endian(numero);
    printf("* Id: %#010x\n", numero);
    return 0;
}

int decoder_rcv_mssg(socket_t *self){
    int socket_close = 0;
    while (true && (socket_close == 0)){
        char header1[16]; 
        socket_close = socket_receive(self, header1, sizeof(header1));
        if (socket_close == 1){
            break;
        }
        int size_array_opt = decoder_get_size_of_array_opt(header1);
        decoder_print_id_number(header1);
        char array_opt[size_array_opt];
        socket_receive(self, array_opt, size_array_opt);
        decoder_print_data(array_opt, size_array_opt);
        int size_body = decoder_get_size_of_body(header1);
        if (size_body != 0){
            char body[size_body];
            socket_receive(self, body, size_body);
            decoder_print_body(body, size_body);
        }
        char mssg_send[3] = "OK\n";
        socket_send(self, mssg_send, sizeof(mssg_send));
    }
    return 0;
}
