#include "common_dbus.h"

int dbus_set_array_opciones(char *clean_line, char *array_opt){
    size_t array_opt_pos = 0;
    size_t read_pos = 0;
    char argument[32];
    memset(argument, 0, sizeof(argument));
    file_get_argument(clean_line, argument, 0);
    read_pos += strlen(argument);
    array_opt_pos += dbus_set_destino(argument, array_opt, array_opt_pos);
    memset(argument, 0, sizeof(argument));
    file_get_argument(clean_line, argument, read_pos+1);
    read_pos += strlen(argument);
    array_opt_pos += dbus_set_ruta(argument, array_opt, array_opt_pos);
    memset(argument, 0, sizeof(argument));
    file_get_argument(clean_line, argument, read_pos+2);
    read_pos += strlen(argument);
    array_opt_pos += dbus_set_interfaz(argument, array_opt, array_opt_pos);
    memset(argument, 0, sizeof(argument));
    file_get_argument(clean_line, argument, read_pos+3);
    array_opt_pos += dbus_set_metodo(argument, array_opt, array_opt_pos);
    int parameters = file_get_amout_of_parametres(clean_line);
    if (parameters != 0){
        array_opt_pos += dbus_set_firma(parameters, array_opt, array_opt_pos);
    }
    return array_opt_pos;
}   

int dbus_get_padding(int argument_length){
    int padding = 0;
    if ((argument_length+1) % 8 == 0){
        padding = 0;
    }else{
        padding = 8 - ((argument_length+1) % 8);
    }
    return padding;
}

int dbus_set_destino(char *destino, char *array_opt, size_t array_opt_pos){
    int padding = dbus_get_padding(strlen(destino));
    unsigned int size_of_buffer = padding + (strlen(destino)+1) + 8;
    char *buffer = (char *) malloc(size_of_buffer);
    memset(buffer, 0, size_of_buffer);
    uint32_t length_destino = to_little_endian(strlen(destino));
    buffer[0] = 6;
    buffer[1] = 1;
    buffer[2] = 's';
    buffer[3] = 0;
    *(int *) &buffer[4] = length_destino;
    for (int i = 0; i < (length_destino); i++){
        buffer[4 + sizeof(length_destino) + i] = destino[i];
    }
    memcpy(&array_opt[array_opt_pos], buffer, size_of_buffer);
    free(buffer);
    return size_of_buffer;
}

int dbus_set_ruta(char *ruta, char *array_opt, size_t array_opt_pos){
    int padding = dbus_get_padding(strlen(ruta));
    unsigned int size_of_buffer = padding + (strlen(ruta)+1) + 8;
    char *buffer = (char *) malloc(size_of_buffer);
    memset(buffer, 0, size_of_buffer);
    uint32_t length_ruta = to_little_endian(strlen(ruta));
    buffer[0] = 1;
    buffer[1] = 1;
    buffer[2] = 'o';
    buffer[3] = 0;
    *(int *) &buffer[4] = length_ruta;
    for (int i = 0; i < (length_ruta); i++){
        buffer[4 + sizeof(length_ruta) + i] = ruta[i];
    }
    memcpy(&array_opt[array_opt_pos], buffer, size_of_buffer);
    free(buffer);
    return size_of_buffer;
}

int dbus_set_interfaz(char *interfaz, char *array_opt, size_t array_opt_pos){
    int padding = dbus_get_padding(strlen(interfaz));
    unsigned int size_of_buffer = padding + (strlen(interfaz)+1) + 8;
    char *buffer = (char *) malloc(size_of_buffer);
    memset(buffer, 0, size_of_buffer);
    uint32_t length_interfaz = to_little_endian(strlen(interfaz));
    buffer[0] = 2;
    buffer[1] = 1;
    buffer[2] = 's';
    buffer[3] = 0;
    *(int *) &buffer[4] = length_interfaz;
    for (int i = 0; i < (length_interfaz); i++){
        buffer[4 + sizeof(length_interfaz) + i] = interfaz[i];
    }
    memcpy(&array_opt[array_opt_pos], buffer, size_of_buffer);
    free(buffer);
    return size_of_buffer;
}

int dbus_set_metodo(char *metodo, char *array_opt, size_t array_opt_pos){
    int padding = dbus_get_padding(strlen(metodo));
    unsigned int size_of_buffer = padding + (strlen(metodo)+1) + 8;
    char *buffer = (char *) malloc(size_of_buffer);
    memset(buffer, 0, size_of_buffer);
    uint32_t length_metodo = to_little_endian(strlen(metodo));
    buffer[0] = 3;
    buffer[1] = 1;
    buffer[2] = 's';
    buffer[3] = 0;
    *(int *) &buffer[4] = length_metodo;
    for (int i = 0; i < (length_metodo); i++){
        buffer[4 + sizeof(length_metodo) + i] = metodo[i];
    }
    memcpy(&array_opt[array_opt_pos], buffer, size_of_buffer);
    free(buffer);
    return size_of_buffer;
}

int dbus_set_firma(int cant_parametros, char *array_opt, size_t array_opt_pos){
    int padding = (26 - cant_parametros) % 8;
    unsigned int size_of_buffer = padding + cant_parametros + 6;
    char *buffer = (char *) malloc(size_of_buffer);
    memset(buffer, 0, size_of_buffer);
    buffer[0] = 9;
    buffer[1] = 1;
    buffer[2] = 'g';
    buffer[3] = 0;
    buffer[4] = cant_parametros;
    for (int i = 0; i < cant_parametros; i++){
        buffer[5+i] = 's'; 
    }
    buffer[5+cant_parametros] = 0;
    memcpy(&array_opt[array_opt_pos], buffer, size_of_buffer);
    free(buffer);
    return size_of_buffer - padding;
}

int dbus_set_body(char *clean_line, char *body){
    int cant_param = file_get_amout_of_parametres(clean_line);
    if (cant_param == 0){
        return 0;
    }
    int pos_line = 0;
    char parameter[32];
    uint32_t pos_body = 0;
    for (int i = 0; i < cant_param; i++){
        memset(parameter, 0, sizeof(parameter));
        pos_line = file_get_parameter(clean_line, parameter, pos_line);
        uint32_t parameter_length = to_little_endian(strlen(parameter));
        *(int *) &body[pos_body] = parameter_length;
        for (int i = 0; i < (parameter_length); i++){
            body[pos_body + sizeof(parameter_length) + i] = parameter[i];
        }
        body[pos_body + sizeof(parameter_length) + parameter_length] = 0;
        pos_body += parameter_length + sizeof(parameter_length) + 1;
    }
    return pos_body;
}

int dbus_set_data_header(uint32_t id_number, char *clean_line, char *header){
    char array_opt[256];
    char body[256];
    memset(array_opt, 0, sizeof(array_opt));
    memset(body, 0, sizeof(body));
    uint32_t body_length = to_little_endian(dbus_set_body(clean_line, body));
    header[0] = 'l';
    header[1] = 1;
    header[2] = 0;
    header[3] = 1;
    * (int *) &header[4] = body_length;
    * (int *) &header[8] = to_little_endian(id_number);
    uint32_t opt_array_length = dbus_set_array_opciones(clean_line, array_opt);
    * (int *) &header[12] = opt_array_length;
    memcpy(&header[16], array_opt, opt_array_length);
    return 16 + opt_array_length;
}

int dbus_init_protocol(const char* path, socket_t *self){
    FILE *fp;
    fp = file_open(path);
    uint32_t id_number = 1;
    int bytes_clean_read = 0, header_length = 0, body_length = 0;
    while (!feof(fp)){
        char header[256], body[128], mssg_rcv[3] = "";
        memset(header, 0, sizeof(header));
        memset(body, 0, sizeof(body));
        fseek(fp, bytes_clean_read, SEEK_SET);
        char *read_line = file_read_line(fp);
        char *clean_line = (char*) malloc(strlen(read_line));
        memset(clean_line, 0, strlen(read_line));
        file_clean_line(read_line, clean_line);
        header_length = dbus_set_data_header(id_number, clean_line, header);
        body_length = dbus_set_body(clean_line, body);
        socket_send(self, header, 16);
        socket_send(self, &header[16], header_length - 16);
        if (body_length != 0){
            socket_send(self, body, body_length);
        }
        socket_receive(self, mssg_rcv, sizeof(mssg_rcv));
        printf("%#010x: %s", id_number, mssg_rcv);
        bytes_clean_read += strlen(clean_line) + 1;
        id_number ++;
        free(clean_line);
    }
    fclose(fp);
    return 0;
}
