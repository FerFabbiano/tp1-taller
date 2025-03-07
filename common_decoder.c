#include "common_decoder.h"
#include "common_others.h"
#include "common_dbus.h"

int decoder_get_size_of_array_opt(char *header){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = header[12+i];
    }
    numero = to_little_endian(numero);
    return numero;
}

int decoder_get_size_of_body(char *header){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = header[4+i];
    }
    numero = to_little_endian(numero);
    if (numero == 0){
        printf("\n");
    }
    return numero;
}

data_t decoder_get_ruta(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memcpy(data.ruta, &array_opt[pos_array_opt + BYTES_INI_OPC], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_destino(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memcpy(data.destino, &array_opt[pos_array_opt + BYTES_INI_OPC], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_interfaz(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memcpy(data.interfaz, &array_opt[pos_array_opt + BYTES_INI_OPC], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_get_metodo(char *array_opt, size_t pos_array_opt, data_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = array_opt[pos_array_opt + 4 + i];
    }
    numero = to_little_endian(numero);
    memcpy(data.metodo, &array_opt[pos_array_opt + BYTES_INI_OPC], numero);
    int padding = dbus_get_padding(numero);
    data.pos_array_opt += padding + numero + 9;
    return data;
}

data_t decoder_manage_type(int type, char *array_opt, 
size_t pos_array_opt, data_t data){
    switch (type){
        case TIPO_RUTA: data = decoder_get_ruta(
            array_opt, pos_array_opt, data); break;
        case TIPO_INTERFAZ: data = decoder_get_interfaz(
            array_opt, pos_array_opt, data); break;
        case TIPO_METODO: data = decoder_get_metodo(
            array_opt, pos_array_opt, data); break;
        case TIPO_DESTINO: data = decoder_get_destino(
            array_opt, pos_array_opt, data); break;
        default: break;
    }
    return data;
}

int decoder_print_data(char *array_opt, size_t size_array_opt){
    data_t data;
    data.pos_array_opt = 0;
    memset(data.destino, 0, sizeof(data.destino));
    memset(data.ruta, 0, sizeof(data.ruta));
    memset(data.metodo, 0, sizeof(data.metodo));
    memset(data.interfaz, 0, sizeof(data.interfaz));
    int parameter_type = 0;
    int amount_parameters = 0;
    while (amount_parameters < 4){
        parameter_type = array_opt[data.pos_array_opt];
        data = decoder_manage_type
        (parameter_type, array_opt, data.pos_array_opt, data);
        amount_parameters ++;
    }
    printf("* Destino: %s\n* Ruta: %s\n* Interfaz: %s\n* Metodo: %s\n",
    data.destino, data.ruta, data.interfaz, data.metodo);
    return 0;
}

body_t decoder_get_parameter(char *body, size_t pos_body, body_t data){
    uint32_t numero = 0;
    char *buff = (char*) &numero;
    for (int i = 0; i < sizeof(int); i++){
        buff[i] = body[pos_body + i];
    }
    numero = to_little_endian(numero);
    for (int i = 0; i < numero; i++){
        data.parametro[i] = body[pos_body+4+i];
    }
    data.pos_body += numero + 5;
    return data;
}

int decoder_print_body(char *body, size_t size_body){
    body_t data;
    data.pos_body = 0;
    memset(data.parametro, 0, sizeof(data.parametro));
    if (size_body == 0){
        return 0;
    }
    printf("* Parametros:");
    printf("\n");
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
    for (int i = 0; i < sizeof(int); i++){
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
        const char array_opt[size_array_opt];
        socket_receive(self, (char*)array_opt, size_array_opt);
        decoder_print_data((char*)array_opt, size_array_opt);
        int size_body = decoder_get_size_of_body(header1);
        if (size_body != 0){
            const char body[size_body];
            socket_receive(self, (char*)body, size_body);
            decoder_print_body((char*)body, size_body);
        }
        char mssg_send[4] = "OK\n";
        socket_send(self, mssg_send, sizeof(mssg_send));
    }
    return 0;
}
