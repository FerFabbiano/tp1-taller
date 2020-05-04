#include "common_file_manage.h"
#include "common_dbus.h"
#include "common_socket.h"

/* Aca yo debería crear el socket, y mandar al servidor el header y el body. */
/* Corro este archivo poniendo en la terminal ./client client0.txt (nombre de un
archivo que se que existe, lo puedo encontrar en este directorio). */

int main(int argc, char const *argv[]){
    socket_t s;
    socket_create(&s);
    socket_connect(&s, "localhost", "7000");
    dbus_init_protocol("client0.txt", &s);
    socket_shutdown(&s, SHUT_RDWR);
    socket_destroy(&s); 
    return 0;
}

/*
int main(int argc, char const *argv[]){
    socket_t s;
    char mssg_rcv[3] = "";
    char mss_send[] = "Soy el cliente.\n";
    char mss_send2[] = "Soy el cliente2.\n";
    socket_create(&s);
    socket_connect(&s, argv[1], argv[2]);
    
    socket_send(&s, mss_send, sizeof(mss_send));      

    socket_send(&s, mss_send2, sizeof(mss_send2));      

    socket_receive(&s, mssg_rcv, sizeof(mssg_rcv));
    
    printf("%s\n", mssg_rcv);
    socket_shutdown(&s, SHUT_RDWR);
    socket_destroy(&s); 
    return 0;
}
*/
