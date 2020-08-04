#include <crypto.h>


int gerate_iv(struct cipher_handle_t * cipher, buffer_t * iv) {
    mbedtls_ctr_drbg_random((void *)&cipher->random_context, iv->buffer, iv->buffer_size);
    return CODE_SUCCESS;
}

int initialize_crypto(struct cipher_handle_t * cipher, uint32_t algorithm, \
        uint32_t mode, char  * key, uint32_t key_size) {
    cipher->aes = NULL;
    if (cipher->random_context == NULL)
        mbedtls_ctr_drbg_init(&cipher->random_context);
    if (cipher->entropy_context == NULL)
        mbedtls_entropy_init(&cipher->entropy_context);
    int res = mbedtls_ctr_drbg_seed(&cipher->random_context, &mbedtls_entropy_func, (void *)&cipher->entropy_context, NULL, 0);
    if (res != 0) {
        mbedtls_entropy_free(&cipher->entropy_context);
        mbedtls_ctr_drbg_free(&cipher->random_context);
        EMSG("Failed to init radom generator");
    }
    return res || initialize_cipher(cipher, algorithm, mode, key, key_size);
}

void finish_crypto(struct cipher_handle_t * cipher) {
    if (cipher->aes != NULL)
        mbedtls_aes_free(&cipher->aes);
    if (cipher->random_context != NULL)
        mbedtls_ctr_drbg_free(&cipher->random_context);
    if (cipher->entropy_context != NULL)
        mbedtls_entropy_free(&cipher->entropy_context);

}

int initialize_cipher(struct cipher_handle_t * cipher, uint32_t algorithm, \
        uint32_t mode, char  * key, uint32_t key_size)
{
    int res;
    
    if (cipher->aes != NULL)
        mbedtls_aes_free(&cipher->aes);

    mbedtls_aes_init(&cipher->aes);

    res = mbedtls_aes_setkey_enc(&cipher->aes, key_size * 8);
    cipher->nonce_counter = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
    cipher->nc_off = 0;

    if (res != 0) {
        EMSG("Failed to allocate operation");
        mbedtls_aes_free(&cipher->aes);
        return res;
    }

    return 0;
}


int decrypt_data(struct cipher_handle_t * cipher, \
                buffer_t * iv, \
                buffer_t * cipher_buffer, \
                buffer_t * plain_buffer)
{

    plain_buffer->buffer = malloc(cipher_buffer->buffer_size);
    plain_buffer->buffer_size = cipher_buffer->buffer_size;
    int res;

    res = mbedtls_aes_crypt_ctr(&cipher->aes, cipher_buffer->buffer_size, &cipher->nc_off, cipher->nonce_counter, iv->buffer, cipher_buffer->buffer, plain_buffer->buffer);
    //gerate_iv(cipher, iv);
    if (res != 0) {
        DMSG("\n    ! ERROR mbedtls_aes_crypt_ctr for decrypt\n");
    }
    return res;

}


int create_encrypted_data(struct cipher_handle_t * cipher, \
                buffer_t * iv, \
                buffer_t * plain_buffer, \
                buffer_t * cipher_buffer)
{

    size_t encrypted_size = plain_buffer->buffer_size;
    unsigned char encrypted_data[encrypted_size];
    int res;

    res = mbedtls_aes_crypt_ctr(&cipher->aes, plain_buffer->buffer_size, &cipher->nc_off, cipher->nonce_counter, iv->buffer, plain_buffer->buffer, encrypted_data);
    gerate_iv(cipher, iv);

    if (res != 0) {
        DMSG("\n    ! ERROR mbedtls_aes_crypt_ctr for encrypt\n");
    }
    
    cipher_buffer->buffer = malloc(encrypted_size);
    memcpy(cipher_buffer->buffer, encrypted_data, encrypted_size);
    cipher_buffer->buffer_size = encrypted_size;
    return res;
}