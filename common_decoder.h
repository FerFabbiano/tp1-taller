#ifndef DECODER_H
#define DECODER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_dbus.h"

typedef struct data{
    char ruta[32];
    char destino[32];
    char metodo[32];
    char interfaz[32];
    int pos_array_opt;
}data_t;

typedef struct body{
    char parametro[32];
    int pos_body;
}body_t;


int funcion();

/* Obtengo el tamaño del body que me van a mandar. Información que
contiene los primeros 16 bytes del header. */
int decoder_get_size_of_body(char *header);

/* Obtengo el tamaño del array de opciones que me van a mandar. Información que
contiene los primeros 16 bytes del header. */
int decoder_get_size_of_array_opt(char *header);

/* Imprimo por pantalla los datos recibidos, sin incluir
los parámetros, si es que los hubiese. */
int decoder_print_data(char *array_opt, size_t size_array_opt);

/* Elijo una opción en base al tipo de parámetro que lei */
data_t decoder_manage_type(int type, char *array_opt,
size_t pos_array_opt, data_t data);

/* Obtengo la ruta y la almaceno en un struct con los datos del llamado. */
data_t decoder_get_ruta(char *array_opt, size_t pos_array_opt, data_t data);

/* Obtengo la interfaz y la almaceno en un struct con los datos del llamado. */
data_t decoder_get_interfaz(char *array_opt, size_t pos_array_opt, data_t data);

/* Obtengo el destino y la almaceno en un struct con los datos del llamado. */
data_t decoder_get_destino(char *array_opt, size_t pos_array_opt, data_t data);

/* Obtengo el metodo y la almaceno en un struct con los datos del llamado. */
data_t decoder_get_metodo(char *array_opt, size_t pos_array_opt, data_t data);

/* Obtengo el tamaño de la firma, si no la hay, devuelvo 0. */
int decoder_get_size_of_firma(char *array_opt, size_t size_array_opt);

/* Obtengo un parametro del body y lo almaceno en un struct. */
body_t decoder_get_parameter(char *body, size_t pos_body, body_t data);

/* Imprimo el número de serie incremental, que esta contenido en los 
primeros 16 bytes del header. */
int decoder_print_id_number(char *header);

/* Imprimo los parámetros contenidos en el body, si es que los hay. */
int decoder_print_body(char *body, size_t size_body);

int decoder_rcv_mssg(socket_t *self);

#endif
