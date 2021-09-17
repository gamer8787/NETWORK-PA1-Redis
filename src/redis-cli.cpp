#include <cstdio>
#include "common.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void backslash_n( char* backslash );

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
    int num_command=0;
    while(fgets(command,sizeof(command),stdin)) {
        char one_line[1024]="";
        char copy[1024];
        strcpy(copy, command);
        if (command[strlen(command)-1]=='\n')
            command[strlen(command)-1]='\0';
        if (strstr(command,"\"")!=NULL){
            char *quote =strtok(copy,"\"");
            quote =strtok(NULL,"\"");
            char *bulk = strtok(command," ");
            int num_bulk = 0; 
            char maker[1000]="";
            while(bulk!=NULL){
                char len_bulk[10]="";
                strcat(maker,dollar);
                sprintf(len_bulk,"%ld",strlen(bulk));
                strcat(maker,len_bulk);
                strcat(maker,rn);
                strcat(maker,bulk);
                strcat(maker,rn);
                num_bulk+=1;
                if ( (bulk+strlen(bulk)+2) == quote-copy+command){ // 확실하지 않음
                    bulk = strtok(NULL,"\"");
                    char *slash = strstr(bulk,"\\");
                    if(slash!=NULL){
                        backslash_n(slash);
                    }
                }
                else{
                bulk = strtok(NULL," ");
                }
            }

            char char_num_bulk[10]="";
            sprintf(char_num_bulk,"%d",num_bulk);
            strcat(one_line,star);
            strcat(one_line,char_num_bulk);
            strcat(one_line,rn);
            strcat(one_line,maker);

        }

        else{
            char *quote =strtok(command,"\"");
            quote =strtok(NULL,"\"");
            char *bulk = strtok(command," ");
            int num_bulk = 0; 
            char maker[1000]="";
            while(bulk!=NULL){
                char len_bulk[10]="";
                strcat(maker,dollar);
                sprintf(len_bulk,"%ld",strlen(bulk));
                strcat(maker,len_bulk);
                strcat(maker,rn);
                strcat(maker,bulk);
                strcat(maker,rn);
                num_bulk+=1;
                bulk = strtok(NULL," ");
            }

            char char_num_bulk[10]="";
            sprintf(char_num_bulk,"%d",num_bulk);
            strcat(one_line,star);
            strcat(one_line,char_num_bulk);
            strcat(one_line,rn);
            strcat(one_line,maker);

        }
        strcat(send,one_line);
        strcat(send,"\r\n");
        num_command+=1;
    }
    
    //printf("send is\n%s\n", send);
    //printf("length is %ld\n", strlen(send));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    char send_test1[1000]="*3\r\n$3\r\nSET\r\n$3\r\nfoo\r\n$5\r\nfkyou\r\n";      


    if (write(client_socket,&send,sizeof(send))==-1){
        perror("write error");
        close(client_socket);
        exit(1);
    }

    char read_message[10000];  
    char read_message2[10000];  
     
    memset(&read_message, 0, sizeof(read_message)); //이거 해줘야 되는데 왜?
    memset(&read_message2, 0, sizeof(read_message2));
    if (read(client_socket,&read_message,sizeof(read_message)-1)==-1){ //size -1?
        perror("read error");
        close(client_socket);
        exit(1);
    }

    //printf("read_message is \n%s\n",read_message);
    //printf("length is %ld\n",strlen(read_message));


    //printf("lenght read is %ld\n\n\n\n",strlen(read_message));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    char *divide = strtok(read_message,"\r");
    for(int i=0;i<num_command;i++){
        switch(*divide){
            case '+':
                printf("%s\n",divide+1);
                divide = strtok(NULL,"\r");
                divide+=1;
            case '-':
                break;
            case ':':
                printf("%s\n",divide+1);
                divide = strtok(NULL,"\r");
                divide+=1;
                break;
            case '$':
                if( strncmp(divide+1,"-",1)==0){
                    printf("\n");
                    break;
                }
                divide = strtok(NULL,"\r");
                printf("%s\n",divide+1);
            case '*':
                break;
            default :    
                printf("errr\n");
                break;
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (close(client_socket)==-1){
       perror("close error");
       exit(1);
   }
    return 0;
}

void backslash_n( char* backslash ) {
    if( strncmp(backslash, "\\n",2) ==0){
        *backslash='\n';
    }
    else{
        perror("error not a \\n \n");
        exit(1);
    }

    backslash++;
    for(;*backslash!='\0';backslash++){
        *backslash=*(backslash+1);
    }
    *backslash='\0';
}

//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

