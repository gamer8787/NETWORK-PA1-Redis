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
    /*
    char command[]="";
    while(fgets(command,sizeof(command),stdin)!=NULL){
        printf("command is %s\n",command);
    }
    */
    
    int client_socket;
    int server_socket;

    struct sockaddr_in clnt_addr;
    struct sockaddr_in serv_addr;

    socklen_t clnt_addr_size;

    
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(6379);

    clnt_addr_size = sizeof(clnt_addr);

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        perror("Socket error");
        exit(1);
    }

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
    
    client_socket = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(client_socket == -1){
        perror("Accept error");
        close(server_socket);
        exit(1);
    }

    char read[10000];   
    //memset(read, 0, sizeof(read)); 
    int length=recv(client_socket,read,10000,0);
    printf("char is %c\n",read[18]);
    printf("char is %c\n",read[19]);
    printf("char is %c\n",read[20]);
    printf("char is %c\n",read[21]);
    printf("char is %s\n",read+18);
    printf("length is %d\n",length);
    if (length==-1){ //size -1?
        perror("read error");
        close(server_socket);
        exit(1);
    }
    printf("read is %s\n",read);

    char read2[1000];   
    //memset(read, 0, sizeof(read)); 
    int length2=recv(server_socket,read2,1000,0);
    printf("length2 is %d\n",length2);
    if (length==-1){ //size -1?
        perror("read2 error");
        close(server_socket);
        exit(1);
    }
    printf("read2 is %s\n",read2);

    char send1[]="*1\r\n$7\r\nCOMMAND\r\n$3\r\nSET\r\n*1\r\n$3\r\nfoo\r\n";
    printf("send1 is %s\n",send1); 
    //strcat(send1,"+PONG");
    if (write(client_socket,send1,sizeof(send1))==-1){
        perror("write error");
        close(server_socket);
        exit(1); 
        }

    char send2[]="+PONG\r\n";
    printf("send2 is %s\n",send2); 
    //strcat(send1,"+PONG");
    if (write(client_socket,send2,sizeof(send2))==-1){
        perror("write error");
        close(server_socket);
        exit(1); 
        }
    
    
    
    close(client_socket);
    close(server_socket);

    return 0;
}

//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