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
    char *ruta = (char*) malloc(numero);
    memcpy(ruta, &array_opt[pos_array_opt + 8], numero);
    data.ruta = (char*) malloc(numero*sizeof(char)+1);
    memcpy(data.ruta, ruta, numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    free(ruta);
    return data;
}

data_t decoder_get_destino(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    char *destino = (char*) malloc(numero);
    memcpy(destino, &array_opt[pos_array_opt + 8], numero);
    data.destino = (char*) malloc(numero*sizeof(char)+1);
    memcpy(data.destino, destino, numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    free(destino);
    return data;
}

data_t decoder_get_interfaz(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    char *interfaz = (char*) malloc(numero);
    memcpy(interfaz, &array_opt[pos_array_opt + 8], numero);
    data.interfaz = (char*) malloc(numero*sizeof(char)+1);
    memcpy(data.interfaz, interfaz, numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    free(interfaz);
    return data;
}

data_t decoder_get_metodo(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < 4; i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    char *metodo = (char*) malloc(numero);
    memcpy(metodo, &array_opt[pos_array_opt + 8], numero);
    data.metodo = (char*) malloc(numero*sizeof(char)+1);
    memcpy(data.metodo, metodo, numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    free(metodo);
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
    free(data.ruta);
    free(data.destino);
    free(data.interfaz);
    free(data.metodo);
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
    printf("* Id: 0x%x\n", numero);
    return 0;
}


int funcion(){
    FILE *file = fopen("hexdump.txt", "r+b");
    char header[16];
    char array_opt[119];
    char aux[1];
    char body[10];
    
    fread(header, 1, 16, file);
    int size_array_opt = decoder_get_size_of_array_opt(header);
    decoder_print_id_number(header);
    
    fread(array_opt, 1, size_array_opt, file);
    decoder_print_data(array_opt, sizeof(array_opt));
    int size_body = decoder_get_size_of_body(header);
    
    fread(aux, 1, 1, file);
    
    fread(body, 1, size_body, file);
    decoder_print_body(body, size_body);
    
    fclose(file);
    return 0;
}

int decoder_rcv_mssg(socket_t *self){
    char header1[16]; 
    socket_receive(self, header1, sizeof(header1));
    int size_array_opt = decoder_get_size_of_array_opt(header1);
    
    char array_opt[size_array_opt];
    socket_receive(self, array_opt, size_array_opt);
    decoder_print_data(array_opt, size_array_opt);
    int size_body = decoder_get_size_of_body(header1);
    
    if(size_body != 0){
        char body[size_body];
        socket_receive(self, body, size_body);
        decoder_print_body(body, size_body);
    }
    
    char mssg_send[] = "OK\n";
    socket_send(self, mssg_send, sizeof(mssg_send));
    
    printf("%i\n", size_array_opt);
    decoder_print_id_number(header1);
    return 0;
}
