#include "common_decoder.h"

int main(int argc, char const *argv[]){
    socket_t s;
    socket_t p;
    char mssg_send[] = "OK\n";
    socket_create(&s);
    socket_bind_and_listen(&s, argv[1]);
    socket_accept(&s, &p); 
    char mssg_rcv[17] = "";
    char mssg_rcv2[18] = "";
    
    socket_receive(&p, mssg_rcv, sizeof(mssg_rcv));
    printf("%s", mssg_rcv);
    
    socket_receive(&p, mssg_rcv2, sizeof(mssg_rcv2));
    printf("%s", mssg_rcv2);
    
    socket_send(&p, mssg_send, sizeof(mssg_send));
    
    socket_shutdown(&s, SHUT_RDWR);
    socket_shutdown(&p, SHUT_RDWR);
    socket_destroy(&s);
    socket_destroy(&p);

    return 0;
}
/*
int main(int argc, char* argv[]) {
    funcion();
    return 0;
}
*/
