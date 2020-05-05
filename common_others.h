#ifndef OTHERS_H
#define OTHERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h> 
#include <byteswap.h>

/* Recibo por parámetro un número, y chequeo si el mismo esta guardado 
como little endian. Caso negativo, lo convierto a tal. */
uint32_t to_little_endian(uint32_t number);

#endif
