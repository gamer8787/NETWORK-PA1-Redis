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
    char rn[5]="\r\n";
    char command[1024];
    char dollar[2]="$";
    char star[2]="*";
    while(fgets(command,sizeof(command),stdin)!=NULL){
        if (command[strlen(command)-1]=='\n')
            command[strlen(command)-1]='\0';
        if(strncmp(command,"PING ",(strlen(command)) <5 ? 4 : 5)==0){
            if(strncmp(command+4,"                                       ",strlen(command)-4)==0){
                char str_len[10]="";
                strcat(send,star);
                strcat(send,"1");
                strcat(send,rn);
                strcat(send,dollar);
                sprintf(str_len, "%ld",strlen(command)); 
                strcat(send,str_len);
                strcat(send,rn);
                strcat(send,command);
                strcat(send,rn);
            }
            else{ //나중에 ""랑 띄어쓰기 2개 초과일 때 생각해야됨
                char str_len[10]="";
                strcat(send,star);
                strcat(send,"2");
                strcat(send,rn);
                strcat(send,dollar);
                strcat(send,"4");
                strcat(send,rn);
                strcat(send,"PING");
                strcat(send,rn);
                strcat(send,dollar);
                sprintf(str_len, "%ld",strlen(command)-5); 
                strcat(send,str_len);
                strcat(send,rn);
                strcat(send,command+5);
                strcat(send,rn);
            }
        }
        
        else if(strncmp(command,"GET ",4)==0){
            ;
        }
        /*
        else if(strncmp(command,"SET ",4)==0){
            ;
        }
        else if(strncmp(command,"STRLEN ",3)==0){
            ;
        }
        else if(strncmp(command,"DEL ",3)==0){
            ;
        }
        else if(strncmp(command,"EXISTS ",3)==0){
            ;
        }
        */
        else{
            printf("else not yet\n");
            exit(1);
        }
        break;
    }
    /*
    for(int i=0;i<strlen(send);i++){
        printf("send char : %d\n", *(send+i));
    }
    */
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //char send_test4[1000]="*2\r\n$4\r\nPING\r\n$5\r\nhello\r\n\n*1\r\n$4\r\nPING\r\n\n*2\r\n$4\r\nPING\r\n$10\r\nhelloworld\r\n";
    if (write(client_socket,&send,sizeof(send))==-1){
        perror("write error");
        exit(1);
    }

    char read_message[1024];  
    if (read(client_socket,&read_message,1024)==-1){
        perror("read error");
        exit(1);
    }
    //printf("%s\n",read_message);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    char *divide = strtok(read_message,"\r");
    switch(*read_message){
        case '+':
            printf("%s\n",divide+1);
            divide = strtok(NULL,"\r");
        case '-':
            break;
        case ':':
            break;
        case '$':
            divide = strtok(NULL,"\r");
            printf("%s\n",divide+1);
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
    return 0;
}
//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
