#include "common_file_manage.h"
#include "common_dbus.h"
#include "common_socket.h"

int main(int argc, char const *argv[]){
    socket_t s;
    socket_create(&s);
    socket_connect(&s, argv[1], argv[2]);
    if (argc == 4){
        dbus_init_protocol(argv[3], &s);
    }else{
        dbus_init_protocol((const char*)stdin, &s);
    }
    socket_shutdown(&s, SHUT_RDWR);
    socket_destroy(&s); 
    return 0;
}
