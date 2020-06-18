#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

//#include <mbedtls/net_sockets.h>
//#include <mbedtls/ssl.h>
//#include <mbedtls/entropy.h>
//#include <mbedtls/ctr_drbg.h>

//*
#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
// */
//#include "certs/_.herokuapp.com.pem.h"
#include "certs/_.lisha.ufsc.br.pem.h"

//#define ADDR "api-project-iot.herokuapp.com"

#include "my_post.h"


struct sockaddr_in server;

int sockfd;
    
int len = sizeof(server);
int slen;

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

void connect_tcp(int * sockfd, struct sockaddr_in * server, const char * server_addr, const int port) {

    *sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sockfd == -1)
    {
        perror("Socket ");
        exit(1);
    } else
    {
        printf("Socket open - fd: %d\n", *sockfd);
    }
    //(void)&server_addr;
    //struct hostent *hostStructure = gethostbyname("api-project-iot.herokuapp.com");
    //struct hostent *hostStructure = gethostbyname("localhost");
    struct hostent *hostStructure = gethostbyname(server_addr);
    
    printf("Socket open - host name: %s\n", hostStructure->h_name);
    printf("Socket open - host addr length: %d\n", hostStructure->h_length);
    printf("Socket open - host addr type: %d\n", hostStructure->h_addrtype);

    server->sin_family       = AF_INET;
    server->sin_port         = htons(port);
    
    struct in_addr ** addr_list = (struct in_addr **)hostStructure->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++) {
        printf("%s \n", inet_ntoa(*addr_list[i]));
    }
    for (int  i = 0; addr_list[i] != NULL; i++) {
        printf("hostStructure->h_addr_list[%d]: %s\n", i, inet_ntoa(*addr_list[i]));
        //inet_aton(inet_ntoa(*addr_list[i]), &server->sin_addr);
        server->sin_addr = *addr_list[i];
        //server->sin_addr.s_addr  = server_addr;

        memset(server->sin_zero, 0x0, 8);
//      inet_aton("66.94.230.32", &addr);
        if (connect(*sockfd, (struct sockaddr*) server, sizeof(*server)) == -1)
        {
            perror("Error connecting to the server");
            //exit(1);
        } else {
            printf("Sucess Conection TCP/IP\n");
            return;
        }
    }
    exit(1);
}

//
static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    fprintf( (FILE *) ctx, "%s:%04d: %s", file, line, str );
    fflush(  (FILE *) ctx  );
}
//


int main( int argc, char ** argv) 
{

    get_args(argc, argv, server_addr, &port);

    printf( "\n  . server_address: %s, port: %d\n", server_addr, port);
    
    connect_tcp(&sockfd, &server, server_addr, port);

    int ret = 1;
    uint32_t flags;

    //mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;

    /*
     * 0. Initialize the RNG and the session data
     */
    //mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    printf( "\n  . Seeding the random number generator..." );
    
    mbedtls_entropy_init( &entropy );
    const char *pers = "ssl_client1";
    if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *) pers,
                               strlen(pers) ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    printf(" ok\n");

    /*
     * 0. Initialize certificates
     */
    printf( "  . Loading the CA root certificate ..." );
    fflush( stdout );

    //ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) api_ca_crt,
    //                      api_ca_crt_len );
    ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) lisha_ca_crt, \
                          lisha_ca_crt_len );
    if( ret < 0 )
    {
        printf( " failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    printf( " ok (%d skipped)\n", ret );

    /*
     * 1. Start the connection
     */
    printf( "  . Connecting to tcp/%s/%d...", server_addr, port );
    fflush( stdout );
/*
    if( ( ret = mbedtls_net_connect( &server_fd, SERVER_NAME,
                                         SERVER_PORT, MBEDTLS_NET_PROTO_TCP ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_net_connect returned %d\n\n", ret );
        goto exit;
    }
*/
    printf( " ok\n" );

/*
     * 2. Setup stuff
     */
    printf( "  . Setting up the SSL/TLS structure..." );
    fflush( stdout );

    if( ( ret = mbedtls_ssl_config_defaults( &conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret );
        goto exit;
    }

    printf( " ok\n" );

/* OPTIONAL is not optimal for security,
     * but makes interop easier in this simplified example */
    mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_OPTIONAL );
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

    if( ( ret = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret );
        goto exit;
    }

    if( ( ret = mbedtls_ssl_set_hostname( &ssl, HOSTNAME) ) != 0 )
    {
        printf( " failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret );
        goto exit;
    }

    mbedtls_ssl_set_bio( &ssl, &sockfd, mbedtls_net_send, mbedtls_net_recv, NULL );
    //mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL );
    //mbedtls_ssl_set_bio( &ssl, &sockfd, send, recv, NULL );

    /*
     * 4. Handshake
     */
    printf( "  . Performing the SSL/TLS handshake..." );
    fflush( stdout );

    while( ( ret = mbedtls_ssl_handshake( &ssl ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            printf( " failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret );
            goto exit;
        }
    }

    printf( " ok\n" );

    /*
     * 5. Verify the server certificate
     */
    printf( "  . Verifying peer X.509 certificate..." );

    /* In real life, we probably want to bail out when ret != 0 */
    if( ( flags = mbedtls_ssl_get_verify_result( &ssl ) ) != 0 )
    {
        char vrfy_buf[512];

        printf( " failed\n" );
        
        mbedtls_x509_crt_verify_info( vrfy_buf, sizeof( vrfy_buf ), "  ! ", flags );

        printf( "%s\n", vrfy_buf );

        //goto exit;
    }
    else
        printf( " ok\n" );

    /*
     * 6. Write the GET request
     */
    printf( "  > Write to server:" );
    fflush( stdout );

    len = mount_request((char *) buffer_out);

    while( ( ret = mbedtls_ssl_write( &ssl,(const unsigned char *) buffer_out, len ) ) <= 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            //printf( " failed\n  ! mbedtls_ssl_write returned %d\n\n", ret );
            goto exit;
        }
    }

    len = ret;
    printf( " %d bytes written\n\n%s", len, (char *) buffer_out );

    /*
     * 7. Read the HTTP response
     */
    printf( "\n\n  < Read from server:" );
    fflush( stdout );

    do {
        len = sizeof( buffer_in ) - 1;
        memset( buffer_in, 0, LEN_BUFFER);
        ret = mbedtls_ssl_read( &ssl, (unsigned char *) buffer_in, len );

        if( ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE )
            continue;

        if( ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY )
            break;

        if( ret < 0 )
        {
            printf( "failed\n  ! mbedtls_ssl_read returned %d\n\n", ret );
            break;
        }

        if( ret == 0 )
        {
            printf( "\n\nEOF\n\n" );
            break;
        }

        len = ret;
        printf( " %d bytes read\n\n%s\n\n", len, (char *) buffer_in );
    } while( 1 );

    mbedtls_ssl_close_notify( &ssl );

exit:

    //mbedtls_net_free( &server_fd );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    close(sockfd);
    printf("Socket closed \n");

    return 0;
}
