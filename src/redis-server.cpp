#include <cstdio>
#include "common.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    
    int client_socket;
    int server_socket;

    struct sockaddr_in clnt_addr;
    struct sockaddr_in serv_addr;

    socklen_t clnt_addr_size;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        perror("Socket error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(6379);

    if(bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("Binding error");
        close(server_socket);
        exit(1);
    }

    if(listen(server_socket, 100) == -1){
        perror("Listening Error");
        close(server_socket);
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(client_socket == -1){
        perror("Accept error");
        close(server_socket);
        exit(1);
    }

    char read_message[1000000];  
    memset(&read_message, 0, sizeof(read_message)); //

    if (read(client_socket,&read_message,sizeof(read_message)-1)==-1){ //size -1?
        perror("read error");
        close(server_socket);
        exit(1);
    }

    char send[] = "+pong\r\n";
    if (write(client_socket,&send,sizeof(send))==-1){
        perror("write error");
        close(server_socket);
        exit(1);
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
