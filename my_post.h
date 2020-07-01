#include <string.h>

#include "data_handler/data_handler.h"
#include "http_handler/http_handler.h"


/*
2.1. Query data URL: https://iot.lisha.ufsc.br/api/get.php
Body: 2.1 series

2.2. Create series URL for create: https://iot.lisha.ufsc.br/api/create.php
Body: 2.2 series

2.3. Insert data URL: https://iot.lisha.ufsc.br/api/put.php
Body: 2.3 smartdata

versio 1.1
3.1. Create series (Binary) URL for create: https://iot.lisha.ufsc.br/api/create.php
Body: Series
3.2. Insert data (Binary) URL: https://iot.lisha.ufsc.br/api/put.php
Body: SmartData

*/

int mount_body(char * buffer, int (*print)(char *, void*), void * data, struct Credentials * credential) {
    int size_data = sprintf(buffer,"{");
    size_data += print(buffer + size_data, data);
    /*
    if(workflow) {
        printf(" \n size_data: %d", size_data);
        size_data = strrchr(buffer, '}') - buffer;
        size_data += sprintf(buffer+size_data, ", \"workflow\":%s}", workflow);
    }
    */
    if(credential) {
        size_data += credentials_print_json(buffer + size_data, credential);
    }
    size_data += sprintf(buffer + size_data, "}");
    return size_data;
}

int mount_request(char * buff, struct HttpHeader_t * httpHeader, data_type_t data_type, void * data, struct Credentials * credentials) {
    char buffer_data[512];
    
    int size_data = 0;
    if(data_type == SERIE)
        size_data = mount_body(buffer_data, series_print_json, data, credentials);

    if(data_type == RECORD)
        size_data = mount_body(buffer_data, record_print_json, data, credentials);

    if (!size_data)
        return 0;

    printf("size_data: %d\n", size_data);

    httpHeader->content_length = size_data;
    int displacement = mount_http_header(buff, httpHeader);

    memcpy(buff+displacement, buffer_data, size_data);
    return displacement + size_data;
}