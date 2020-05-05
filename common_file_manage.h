#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_socket.h"

/* Obtengo linea del archivo, puede contener bytes que no me interesan. */
char* file_read_line(FILE *fp);

/* Limpio linea. */
int file_clean_line(char *file_line, char *buffer_clean);

/* Obtengo  un argumento de la linea */
int file_get_argument(char *clean_line, char *argument, size_t pos);


/* Obtengo la cantidad de parametros que tiene mi método.
Cantidad de comas que se encuentren en la línea, mas 1. */
int file_get_amout_of_parametres(char *clean_line);

/* Obtengo un parámetro de mi método */
int file_get_parameter(char *clean_line, char *parameter, size_t pos);

/* Abro el archivo en caso de que me pasen uno, y sino devuelvo 
el file pointer del stdin */
FILE* file_open(const char *path);

#endif
