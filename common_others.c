#include "common_others.h"

uint32_t to_little_endian(uint32_t number){
    int i = 1;
    if (*(char*) &i == 1){
        return number; // mi computadora ya es little endian
    }
    return __bswap_32(number);
}
