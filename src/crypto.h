#ifndef _CRYPTO_H
#define _CRYPTO_H

//#include <tee_internal_api.h>
//#include <tee_internal_api_extensions.h>

#include "mbedtls/aes.h" // aes it self
#include "mbedtls/ctr_drbg.h" // pseudorandom generator based on aes
#include "mbedtls/entropy.h" // entropy function for ctr_drbg

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "defines.h"

struct cipher_handle_t {
    mbedtls_aes_context aes;
    unsigned char nonce_counter[16];
    size_t nc_off;
    mbedtls_ctr_drbg_context random_context;
    mbedtls_entropy_context entropy_context;
    //char* key;
    //uint32_t key_size;
};

int gerate_iv(buffer_t * iv);

int initialize_crypto(struct cipher_handle_t * cipher, uint32_t algorithm, \
        uint32_t mode, char  * key, uint32_t key_size);

void finish_crypto(struct cipher_handle_t * cipher);

int initialize_cipher(struct cipher_handle_t * cipher, uint32_t algorithm, \
        uint32_t mode, char  * key, uint32_t key_size);

int decrypt_data(struct cipher_handle_t * cipher, \
                buffer_t * iv, \
                buffer_t * cipher_buffer, \
                buffer_t * plain_buffer);

int create_encrypted_data(struct cipher_handle_t * cipher, \
                buffer_t * iv, \
                buffer_t * plain_buffer, \
                buffer_t * cipher_buffer);

/*
void print_buffer(buffer_t * buffer);
void print_buffer(buffer_t * buffer) {
    for (uint32_t i = 0; i< buffer->buffer_size; i++)
        DMSG("(%d) %x", i, buffer->buffer[i]);
}
*/
#endif // _CRYPTO_H