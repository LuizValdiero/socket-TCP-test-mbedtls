#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

//#include "certs/_.herokuapp.com.pem.h"
#include "../certs/_.lisha.ufsc.br.pem.h"

#include "./my_post.h"
#include "./connections_handler.h"


#define HOSTNAME "iot.lisha.ufsc.br"
#define PORT 443
#define LEN_BUFFER 2048

unsigned char buffer_in[LEN_BUFFER];
unsigned char buffer_out[LEN_BUFFER];

#define SERVER_ADDR_SIZE 1024
char server_addr[SERVER_ADDR_SIZE];
int port;

void get_args(int argc, char ** argv, char * server_addr, int * port) {
    if (argc == 3) {
        *port = atoi(argv[2]);
        memset(server_addr, '\0', SERVER_ADDR_SIZE);
        strcpy(server_addr, argv[1]);
        return;
    }
    memset(server_addr, '\0', SERVER_ADDR_SIZE);
    memcpy(server_addr, HOSTNAME, sizeof(HOSTNAME));
    *port = PORT;
} 

int main( int argc, char ** argv) 
{

    get_args(argc, argv, server_addr, &port);

    printf( "\n  . server_address: %s, port: %d\n", server_addr, port);
    
    if (open_connections(server_addr, port, HOSTNAME, lisha_ca_crt, lisha_ca_crt_len)) {
        printf("\nerror open_connections\n");
        exit(1);
    }
    
    memset(buffer_out, 0, LEN_BUFFER);
    const char * hostname = HOSTNAME;

    struct HttpHeader_t httpHeader = { \
        .method = POST, \
        .path = API_GET, \
        .content_type = JSON, \
        .hostname = hostname, \
        .content_length = 0};
    
    struct Serie serie = {17, 2224179556, 741868840, 679816441, \
                    25300, 0, 0, 1567021716000000, 1567028916000000};
    struct Credentials credentials = {"smartlisha", "", ""};

    int len = mount_request((char *) buffer_out, LEN_BUFFER, &httpHeader, SERIE, &serie, &credentials);

    len = send_data(buffer_out, len);

    printf( " %d bytes written\n---\n %s\n---\n\n", len, (char *) buffer_out);

    while(1) {
        len = recv_data(buffer_in, LEN_BUFFER);
        if(len < 0) {
            printf("\n exit while recv_data\n");
            break;
        }
        printf( "%s", (char *) buffer_in );
    }

    printf("\nfinish recv");
    close_conections();

    return 0;
}
