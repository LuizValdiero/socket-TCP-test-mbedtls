#ifndef _TLS_HANDLER_H
#define _TLS_HANDLER_H

#include "defines.h"

#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/ssl.h>
#include <mbedtls/x509.h>
#include <mbedtls/debug.h>
#include <mbedtls/certs.h>
#include <mbedtls/error.h>

int tls_handler_write(mbedtls_ssl_context * ssl, unsigned char * buffer, size_t size);
int tls_handler_read(mbedtls_ssl_context * ssl, unsigned char * buffer, size_t size);

void tls_print_error_code(int error_code);
void tls_print_x509_crt_verify_info (int flags);

void initialize_tls_structures(mbedtls_ssl_context* ssl, \
                mbedtls_ssl_session* ssl_sess, \
                mbedtls_ssl_config* conf, \
                mbedtls_entropy_context* entropy, \
                mbedtls_ctr_drbg_context* ctr_drbg, \
                mbedtls_x509_crt* cacert);
void finish_tls_structures(mbedtls_ssl_context* ssl, \
                mbedtls_ssl_config* conf, \
                mbedtls_entropy_context* entropy, \
                mbedtls_ctr_drbg_context* ctr_drbg, \
                mbedtls_x509_crt* cacert);
int initialize_ctr_drbg(mbedtls_entropy_context * entropy, \
	            mbedtls_ctr_drbg_context * ctr_drbg, \
                const char * pers, \
                int(*f_entropy)(void *, unsigned char *, size_t));
int set_ca_root_certificate(mbedtls_x509_crt * cacert, \
                const unsigned char * api_ca_crt, \
                size_t api_ca_crt_len);
int setting_up_tls(mbedtls_ssl_config* conf, \
                mbedtls_ctr_drbg_context* ctr_drbg, \
                mbedtls_x509_crt* cacert, \
                int(*f_rng)(void *, unsigned char *, size_t));
int assign_configuration(mbedtls_ssl_context * ssl, mbedtls_ssl_config * conf);
int set_hostname(mbedtls_ssl_context * ssl ,const char * hostname);
void set_bio(mbedtls_ssl_context * ssl, void  * sess_socket, \
                        mbedtls_ssl_send_t *f_send, mbedtls_ssl_recv_t *f_recv, \
                        mbedtls_ssl_recv_timeout_t *f_recv_timeout);
int handshake(mbedtls_ssl_context * ssl);
int verify_server_certificate(mbedtls_ssl_context * ssl);

int tls_is_connected(mbedtls_ssl_context* ssl);
int tls_reconnect(mbedtls_ssl_context* ssl, mbedtls_ssl_session* ssl_sess);

#endif // _TLS_HANDLER_H