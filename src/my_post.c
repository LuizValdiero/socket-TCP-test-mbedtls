#include "my_post.h"

int mount_body(buffer_t * out, \
                int (*data_to_json)(buffer_t * out, int *displacement, buffer_t * data), \
                buffer_t * data, struct credentials_t * credential)
{
    int displacement = snprintf((char *)  out->buffer, out->buffer_size, "{");
    data_to_json(out, &displacement, data);
    /*
    if(workflow) {
        printf(" \n size_data: %d", size_data);
        size_data = strrchr(buffer, '}') - buffer;
        size_data += sprintf(buffer+size_data, ", \"workflow\":%s}", workflow);
    }
    */
    if (credential) {
        credentials_print_json(out, &displacement, credential);
    }
    displacement += snprintf((char *)  out->buffer + displacement ,out->buffer_size - displacement, "}");
    out->buffer_size = displacement;
    return CODE_SUCCESS;
}

int mount_request(buffer_t * out, struct HttpHeader_t * httpHeader, \
                    int (*data_to_json)(buffer_t * out, int *displacement, buffer_t * data), \
                    buffer_t * data, struct credentials_t * credentials)
{
    buffer_t body = { .buffer = malloc(out->buffer_size), \
                        .buffer_size = out->buffer_size};
    if (!body.buffer)
        return CODE_ERROR_OUT_OF_MEMORY;//TEE_ERROR_OUT_OF_MEMORY;
    
    mount_body(&body, data_to_json, data, credentials);
    httpHeader->content_length = body.buffer_size;
    int displacement = 0;
    mount_http_header(out, &displacement, httpHeader);
    if (out->buffer_size < displacement + body.buffer_size) {    
        free(body.buffer);
        return CODE_ERROR_SHORT_BUFFER;//TEE_ERROR_SHORT_BUFFER;
    }
    memcpy(out->buffer + displacement, body.buffer, body.buffer_size);
    out->buffer_size = displacement + body.buffer_size;
    free(body.buffer);
    return CODE_SUCCESS;
}
