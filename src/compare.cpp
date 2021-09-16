#include <cstdio>
#include "common.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int index(char * str, char c);
int main(int argc, char *argv[])
{
    struct hostent* host;
    struct sockaddr_in server;
    int client_socket;
  
    client_socket = socket(PF_INET,SOCK_STREAM,0);
    if(client_socket == -1){
        perror("socekt_error") ;
        exit(1);
    } 
    if(strlen(argv[3]) > 20){ // ALL TEST
        if((host = gethostbyname(argv[3])) == NULL){
            perror("connect error");
            close(client_socket);
            exit(1);
        }
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        memcpy(&server.sin_addr,host->h_addr,host->h_length);
        server.sin_port = htons(6379);
    }
    else{ // LOCAL TEST
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(argv[2]);
        server.sin_port = htons(6379);
    }

    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) ==-1 ){
        close(client_socket);
        perror("connect error");
        exit(1);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // make resp form
    char send[1024]="";
    char len_string[5]; 
    char bulk[1024];
    char blank[1024]="";
    char rn[5] = "\r\n";
    char star[10] = "*";
    int num_bulk=0;
    while(fgets(bulk,sizeof(bulk),stdin)!=NULL){
        char len_bulk[10]="";
        char dollar[10]="$";
        if (bulk[strlen(bulk)-1]=='\n')
            bulk[strlen(bulk)-1]='\0';
        sprintf(len_bulk, "%d",strlen(bulk)); 
        strcat(dollar,len_bulk);
        strcat(blank,dollar);
        strcat(blank,rn);
        strcat(blank,bulk);
        strcat(blank,rn);
        num_bulk++;
    }
    char char_num_bulk[10];
    sprintf(char_num_bulk, "%d",num_bulk); 
    strcat(star,char_num_bulk);
    strcat(star,rn);

    strcat(send, star);
    strcat(send, blank);
    /*
    for(int i=0;i<strlen(send);i++){
        printf("send char : %d\n", *(send+i));
    }
    */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    char send_test4[1000]="*2\r\n$4\r\nPING\r\n$5\r\nhello\r\n\n*1\r\n$4\r\nPING\r\n\n*2\r\n$4\r\nPING\r\n$10\r\nhelloworld\r\n";
    if (write(client_socket,&send_test4,sizeof(send_test4))==-1){
        perror("write error");
        exit(1);
    }

    char read_message[1024];  
    if (read(client_socket,&read_message,1024)==-1){
        perror("read error");
        exit(1);
    }
    printf("message1111 is %s\n",read_message);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    char *divide = strtok(read_message,"\r");
    switch(*read_message){
        case '+':
            //printf("%s\n",divide+1);
            divide = strtok(NULL,"\r");
        case '-':
            break;
        case ':':
            break;
        case '$':
            divide = strtok(NULL,"\r");
            //printf("%s\n",divide+1);
        case '*':
            break;
        default :    
            printf("errr\n");
    }
   
    if (close(client_socket)==-1){
       perror("close error");
       exit(1);
   }
    return 0;
}

int index(char * str, char c){
    for(int i=0;i<strlen(str);i++){
        if (*(str+i) == c ){
            return i;
        }
    }
}
//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