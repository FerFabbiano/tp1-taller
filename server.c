#include "common_decoder.h"

int main(int argc, char* argv[]) {
    socket_t s;
    socket_t p;
    socket_create(&s);
    socket_bind_and_listen(&s, argv[1]);
    socket_accept(&s, &p); 
    decoder_rcv_mssg(&p);
    socket_shutdown(&s, SHUT_RDWR);
    socket_shutdown(&p, SHUT_RDWR);
    socket_destroy(&s);
    socket_destroy(&p);
    return 0;
}

