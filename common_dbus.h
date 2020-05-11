#ifndef DBUS_H
#define DBUS_H

#include <stdlib.h>
#include "common_socket.h"

#define TIPO_RUTA 1
#define TIPO_DESTINO 6
#define TIPO_INTERFAZ 2
#define TIPO_FIRMA 9
#define TIPO_METODO 3
#define CANT_STRINGS 1
#define BYTES_INI_OPC 8
#define BYTE_FIN_STRING 0

/* Es un número que surge ante la necesidad de calcular el padding para este 
caso en particular. Asumo que no voy a tener mas de 26 parámetros. 
(34 - cant.parametros) % 8 = padding. Esto resulta asi ya que para la firma, 
se va a agregar un string 's' por cada parámetro. Por tanto tengo los 4 bytes
iniciales de la opción, mas el byte indicanco la cantidad de parámetros,
mas un byte por cada parámetros indicando el tipo del mismo.  */
#define PADDING_ARGUMENTOS 34

/* Inicializo el protocolo, leo linea proceso, y envio */
int dbus_init_protocol(const char *path, socket_t *self);

/*
Setea los primeros 16 bytes del header según el protocolo a implementar. 
Copio el array de opciones en las posiciones siguientes.
Copio el body al al finalizar el header.
*/
int dbus_set_data_header(uint32_t id_number, char *clean_line, char *header);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_destino(char *destino, char *buffer_send, size_t array_opt_pos);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_ruta(char *ruta, char *array_opt, size_t array_opt_pos);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_interfaz(char *interfaz, char *array_opt, size_t array_opt_pos);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_metodo(char *metodo, char *array_opt, size_t array_opt_pos);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_array_opciones(char *clean_line, char *array_opt);

/* costruyo un array con los datos de la opción y la 
almaceno en el array de opciones */
int dbus_set_firma(int cant_parametros, char *array_opt, size_t array_opt_pos);

/* Obtengo la cantidad de bytes de padding que necesito 
para el array de un parametro. */
int dbus_get_padding(int argument_length);

#endif
