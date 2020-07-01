#include "../../my_post.h"

#include "../../data_handler/data_handler.h"
#include "../../http_handler/http_handler.h"

int main(int argc, char const *argv[])
{
    char buffer_out[512];
    memset(buffer_out, 0, 512);
    const char * hostname = "iot.lisha.ufsc.br";

    struct HttpHeader_t httpHeader = { \
        .method = POST, \
        .path = API_GET, \
        .content_type = JSON, \
        .hostname = hostname, \
        .content_length = 0};
    
    struct Serie serie = {17, 2224179556, 741868840, 679816441, \
                    25300, 0, 0, 1567021716000000, 1567028916000000};
    struct Credentials credentials = {"smartlisha", "", ""};

    int size = mount_request((char *) buffer_out, &httpHeader, SERIE, &serie, &credentials);
    printf("\n---\n%s\n---\n", buffer_out);
    return 0;
}