
#include "data_handler.h"


//const int size_serie = (( 8 + 6*32 + 2*64) >> 2);
//const int size_record = (8 + 6*32 + 64 + (double)) >>2;

const int size_data_list[] = { size_serie, size_record};


//(int (*print)(char * buffer, void * data)) print_functions[] = {series_print, record_print};

int write_size_and_value(char * buffer, char * value){
    int8_t size = 0;
    const int byte_size = 1;
    if (value){
        size = strlen(value);
        memcpy(buffer+byte_size, value, size);
    }
    memset(buffer, size, 1);
    return (byte_size + size);
}

int get_version_high(uint8_t version) {
    return (version & 0xf0)>>4;
}

int get_version_low(uint8_t version) {
    return version & 0x0f;
}

int credentials_print(char * buffer, struct Credentials * credentials) {
    int displacement = 0;
    displacement += write_size_and_value(buffer, credentials->domain);
    displacement += write_size_and_value(buffer + displacement, credentials->username);
    displacement += write_size_and_value(buffer + displacement, credentials->password);
    return displacement;
}

int credentials_print_json(char * buffer, struct Credentials * credentials)
{
    return sprintf(buffer, ", \"credentials\": { \"domain\":\"%s\"," \
        " \"username\":\"%s\", \"password\":\"%s\"}", \
        credentials->domain, credentials->username, credentials->password);
}

int series_print(char * buffer, void * data)
{
    memcpy(buffer, (struct Serie *)data, size_data_list[SERIE]);
    return size_data_list[SERIE];
}

int series_print_json(char * buffer, void * data){
    struct Serie * serie = (struct Serie *) data;
    return sprintf(buffer, \
        "\"series\": {\"version\": \"%d.%d\", " \
        "\"unit\": %u, \"x\": %d, \"y\": %d, \"z\": %d, " \
        "\"dev\": %d,  \"r\": %u, " \
        " \"t0\": %lu, \"t1\": %lu}", \
        get_version_high(serie->version), get_version_low(serie->version), \
        serie->unit, serie->x, serie->y, serie->z, \
        serie->dev, serie->r, \
        serie->t0, serie->t1);
}

int record_print(char * buffer, void * data)
{
    memcpy(buffer, (struct Record *)data, size_data_list[RECORD]);
    return size_data_list[RECORD];
}

int record_print_json(char * buffer, void * data) {
    struct Record * record = (struct Record *) data;
    return sprintf(buffer, \
        "\"smartdata\": [{\"version\": \"%d.%d\", " \
        "\"unit\": %u, \"value\": %f, \"uncertainty\": %u, "\
        "\"x\": %d, \"y\": %d, \"z\": %d, " \
        "\"t\": %lu, \"dev\": %d}]", \
        get_version_high(record->version), get_version_low(record->version), \
        record->unit, record->value, record->uncertainty, \
        record->x, record->y, record->z, \
        record->t, record->dev);    
}

int smartdata_to_json(char * buffer) {
    const char smartdata_model[] = {"\"smartdata\" : [ RECORDS ]"};
    memcpy(buffer, smartdata_model, sizeof(smartdata_model));
    return 0;    
}