#include "common_file_manage.h"

#define BUFF_READ_SIZE 32

char* file_read_line(FILE *fp){
    char buffer[BUFF_READ_SIZE] = "";
    memset(buffer, 0, sizeof(buffer));
    fread(buffer, 1, sizeof(buffer), fp);
    char *full_line = (char*) malloc(BUFF_READ_SIZE);
    memcpy(full_line, buffer, sizeof(buffer));
    int n = 2;
    while (strchr(full_line, '\n') == NULL){
        memset(buffer, 0, sizeof(buffer));
        full_line = realloc(full_line, n*BUFF_READ_SIZE);
        fread(buffer, 1, sizeof(buffer), fp);
        memcpy(&full_line[(n-1)*BUFF_READ_SIZE], buffer, sizeof(buffer));
        n++;
    }
    return full_line;
}

int file_clean_line(char *file_line, char *buffer_clean){
    int i = 0;
    while (file_line[i] != '\n'){
        buffer_clean[i] = file_line[i];
        i++;
    }
    return 0;
}

int file_get_argument(char *clean_line, char *argument, size_t pos){
    for (int i = 0; (clean_line[pos] != ' ') && (clean_line[pos] != '(')
    && (clean_line[pos] != ',') && (clean_line[pos] != ')'); i++){
        argument[i] = clean_line [pos];
        pos ++;
    }
    return 0;
}

int file_get_amout_of_parametres(char *clean_line){
    size_t i = 0;
    size_t cant_parametros = 0;
    while (i < strlen(clean_line)){
        if (clean_line[i] == '(' && clean_line[i+1] == ')'){
            return 0;
        }
        if (clean_line[i] == ',' || clean_line[i] == '('){
            cant_parametros += 1;
            i++;
        } else{
            i++;
        }
    }
    return cant_parametros;
}

int file_get_parameter(char *clean_line, char *parameter, size_t pos){
    while ((clean_line[pos] != '(') && (clean_line[pos] != ',')){
        pos ++;
    }
    pos ++;
    int i = 0;
    while ((clean_line[pos] != ',') && (clean_line[pos] != ')')){
        parameter[i] = clean_line [pos];
        pos ++;
        i++;
    }
    return pos;
}

FILE* file_open(const char *path){
    FILE *fp;
    if (path == NULL){
        fp = stdin;
    }else{
        fp = fopen(path, "r");
    }
    if (fp == NULL){
        return NULL;
    }
    return fp;
}
