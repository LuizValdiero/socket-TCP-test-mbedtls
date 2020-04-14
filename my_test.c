//
// https://blog.pantuza.com/artigos/programando-um-protocolo-utilizando-sockets
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define ADDR "192.168.1.106"
#define PORT 4433

#define LEN_BUFFER 2048

struct sockaddr_in server;

int sockfd;
    
int len = sizeof(server);
int slen;

char buffer_in[LEN_BUFFER];
char buffer_out[LEN_BUFFER];


int main( int argc, char ** argv) 
{
    sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
    {
        perror("Socket ");
        exit(1);
    } else
    {
        printf("Socket open - fd: %d\n", sockfd);
    }
    
    server.sin_family       = AF_INET;
    server.sin_port         = htons(4433);
    server.sin_addr.s_addr  = inet_addr(ADDR);

    memset(server.sin_zero, 0x0, 8);

    if (connect(sockfd, (struct sockaddr*) &server, len) == -1)
    {
        perror("Error connecting to the server");
        exit(1);
    }
    len = strlen("Hello World");
    memcpy(buffer_out, "Hello World", len);
    buffer_out[len] = '\0';
    slen = send(sockfd, buffer_out, len, 0);
    printf("Sent (%d bytes): %s\n", slen, buffer_out);

    if ((slen = recv(sockfd, buffer_in, LEN_BUFFER, 0)) > 0)
    {
        buffer_in[slen + 1] = '\0';
        printf("Server: %s\n", buffer_in);
    } else
    {
        printf("Error recv\n");
    }

    close(sockfd);

    printf("Socket closed \n");

    return 0;
}