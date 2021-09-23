#include <cstdio>
#include "common.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
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

    if(listen(server_socket, 5) == -1){
        perror("Listening Error");
        close(server_socket);
        exit(1);
    }
        //11111111111111111111
        client_socket = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(client_socket == -1){
            perror("accept error");
            close(server_socket);
            }
        char read[1000];   
        memset(read, 0x00, sizeof(read)); 
        if (recv(client_socket,read,sizeof(read),0)==-1){ //size -1?
            perror("read error");
            close(server_socket);
            exit(1);
        }
        printf("read is %s\n",read);
              
        char send3[100]="-ERR\r\n";
        memset(send3, 0x00, sizeof(send3)); 
        printf("send3 is %s\n",send3); 
        if (write(client_socket,send3,strlen(send3))==-1){
            perror("write error");
            close(server_socket);
            exit(1); 
        }
        //////////
        close(client_socket);
        //222222222222222222
        client_socket = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(client_socket == -1){
            perror("accept error");
            close(server_socket);
            }
        char read2[1000];   
        memset(read2, 0x00, sizeof(read2)); 
        if (recv(client_socket,read2,sizeof(read2),0)==-1){ //size -1?
            perror("read2 error");
            close(server_socket);
            exit(1);
        }
        printf("read2 is %s\n",read2);
        
        char send2[50]="+PONG\r\n";
        printf("lens is %ld\n",strlen(send2));
        //memset(send2, 0x00, sizeof(send2)); 
        printf("send2 is %s\n",send2); 
        if (write(client_socket,send2,strlen(send2))==-1){
            perror("write error");
            close(server_socket);
            exit(1); 
        }
        
       
        
        close(client_socket); //2번째
        
        //333333333333333
        int c2;
        c2 = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(c2 == -1){
            printf("hi\n");
            close(server_socket);
            exit(1);
            }
        
        
        char read3[1000];   
        memset(read3, 0x00, sizeof(read3)); 
        if (recv(c2,read3,sizeof(read3),0)==-1){ //size -1?
            printf("hi\n");
            close(server_socket);
            exit(1);
        }
        printf("read3 is %s\n",read3);
        
        char send1[50]=":25\r\n";
        printf("send1 is %s\n",send1); 
        //strcat(send1,"+PONG");
        if (write(client_socket,send1,strlen(send1))==-1){
            perror("write error");
            close(server_socket);
            exit(1); 
        }
        close(client_socket);
    close(server_socket);

    return 0;
}

//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
