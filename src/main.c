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
#include "../credentials/secret_credentials.h"

#include "defines.h"
#include "my_post.h"
#include "connections_handler.h"
#include "serial_package.h"
//#include "crypt.h"


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

int create_package(uint8_t data_code,
	buffer_t * package_out)
{
    int res;
	serial_header_t header;
	buffer_t encrypted_data;
	buffer_t plain_data;

	uint32_t total_size = 0;

	if (data_code == 'S') {
		serie_t serie = { \
					.version = 17, \
					.unit = 2224179556, \
					.x = 0, \
					.y = 1, \
					.z = 2, \
					.dev = 0, \
					.r = 0, \
					.t0 = 1594080000000000, \
					.t1 = 1594291176706000 };
		res = create_data_package(SERIE, &plain_data, (void *) &serie);
	} else {
		record_t record = { \
					.version = 17, \
					.unit = 2224179556, \
					.value = 15.03, \
					.uncertainty = 0, \
					.x = 0, \
					.y = 1, \
					.z = 2, \
					.t = 1594256176706000, \
					.dev = 0};
		res = create_data_package(RECORD, &plain_data, (void *) &record);	
	}

    if(res != CODE_SUCCESS) {
        return res;
    }

    unsigned char iv_char[16];
	buffer_t iv = { .buffer_size = 16, .buffer = iv_char};
	
    //gerate_iv(&iv);
	//create_encrypted_data(&tls_handle->cipher, &iv, \//
    //            &plain_data, &encrypted_data);
	
    //free(plain_data.buffer);
    encrypted_data.buffer = plain_data.buffer;
    encrypted_data.buffer_size = plain_data.buffer_size;
    //


	total_size = sizeof(header) + encrypted_data.buffer_size;
	
    if (package_out->buffer_size < total_size) {
		free(encrypted_data.buffer);
		return CODE_ERROR_SHORT_BUFFER;
	}

	header.encrypted_size = encrypted_data.buffer_size;
	memcpy(header.iv, iv.buffer, iv.buffer_size);

	mount_serial_package(package_out, &header, &encrypted_data);
	package_out->buffer_size = total_size;
    
	free(encrypted_data.buffer);
    
	return 0;
}

int send_package( buffer_t * package_in, int * response_code, \
                struct HttpHeader_t * httpHeader, struct credentials_t * credentials)
{
	//struct tls_handle_t *tls_handle = (struct tls_handle_t *)sess_ctx;
	int ret;
	
	buffer_t encrypted_data;
	buffer_t plain_data;
	serial_header_t header;
	
	dismount_serial_package(package_in, &header, &encrypted_data);
	
    //buffer_t iv;
	//iv.buffer = header.iv;
	//iv.buffer_size = sizeof(header.iv);
	//decrypt_data(&tls_handle->cipher, &iv, \//
    //            &encrypted_data, &plain_data);

    //free(encrypted_data.buffer);
    plain_data.buffer = encrypted_data.buffer;
    plain_data.buffer_size = encrypted_data.buffer_size;
    //
    

	int request_size = 512;
	buffer_t request = { .buffer = malloc(request_size), .buffer_size = request_size};

	path_t path = get_request_path_of_data_package(&plain_data);
	set_request_path_in_header(httpHeader, path);
    
	ret = mount_request( &request, httpHeader, data_package_to_json, &plain_data, credentials);

    //free(plain_data.buffer);
    if(ret != CODE_SUCCESS) {
        printf("\n    ! Error: mount_request\n");
        //free(plain_data.buffer);
        free(request.buffer);
        return ret;
    }

	ret = send_data(request.buffer, request.buffer_size);	
    free(request.buffer);

	if(ret <  0)
		return 1;//TEE_ERROR_COMMUNICATION;

	#define HTTPS_RESPONSE_BUFFER_SIZE 1024
	buffer_t response = {.buffer = malloc(HTTPS_RESPONSE_BUFFER_SIZE), .buffer_size = HTTPS_RESPONSE_BUFFER_SIZE};

	int response_size = recv_data(response.buffer, response.buffer_size);
	if (response_size >= 0) {
		*response_code = get_response_code(&response);
		while (response_size > 0)
		{
			response_size = recv_data(response.buffer, response.buffer_size);
		}
	}

	if (!*response_code)
		return 1;//TEE_ERROR_CANCEL;
	return 0;
}


int main( int argc, char ** argv) 
{

    get_args(argc, argv, server_addr, &port);

    printf( "\n  . server_address: %s, port: %d\n", server_addr, port);
    
    if (open_connections(server_addr, port, HOSTNAME, lisha_ca_crt, lisha_ca_crt_len)) {
        printf("\n  ! Error: open_connections\n");
        exit(1);
    }
    printf("\n  . open_connections success\n");
    //const char * hostname = HOSTNAME;
   struct HttpHeader_t httpHeader = { \
        .method = POST, \
        .path = API_GET, \
        .content_type = JSON, \
        .hostname = HOSTNAME, \
        .content_length = 0};
    
    struct credentials_t credentials = { \
		.domain = SECRET_DOMAIN, \
		.username = SECRET_USERNAME, \
		.password = SECRET_PASSWORD};
/*
    char key[16] = {0x99, 0xF3, 0xCC, 0xA3, 0xFC, 0xC7, 0x10, 0x76, 0xAC, 0x16,
          0x86, 0x41, 0xD9, 0x06, 0xCE, 0xB5};
	int key_size = 16;
	uint32_t algorithm = TEE_ALG_AES_CTR;

	initialize_crypto(&tls_handle->cipher, algorithm, TEE_MODE_ENCRYPT, key, key_size);
*/
    unsigned char package_char[1024];
    buffer_t package_data = { .buffer_size = 1024, .buffer = package_char};
    if (create_package('S', &package_data) != CODE_SUCCESS) {
        printf("\nError: create package");
        return 0;
    }

    printf("\n  . create_package success");

    int response_code;
    send_package( &package_data, &response_code, &httpHeader, &credentials);

    printf("\nfinish recv, response code: %d", response_code);
    close_conections();
	//finish_crypto(&tls_handle->cipher);

    return 0;
}
