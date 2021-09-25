#include <cstdio>
#include "common.hpp"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER 500
int main(int argc, char *argv[])
{
    struct hostent* host;
    struct sockaddr_in server;
    int client_socket;
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
    /////////////////////////////////////////////////////////////////

    char* end;
    int first=0;
    while(first!=-1){
        client_socket = socket(PF_INET,SOCK_STREAM,0);
        if(client_socket == -1){
            perror("socekt_error") ;
            exit(1);
        } 

        if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) ==-1 ){
            close(client_socket);
            perror("connect error");
            exit(1);
        }
        char send[MAX_BUFFER]="";
        memset(send,0,sizeof(send)); //
        int num_command=0;
        
        make_resp_form( send, &first, end, &num_command);

        if (write(client_socket,&send,strlen(send))==-1){
            perror("write error");
            close(client_socket);
            exit(1);
        }
        int mode=1;
        int i=1;
        int num_bulk=0;
        int len_num=0;
        while(1){
            char read_message[MAX_BUFFER];  
            memset(&read_message, 0, sizeof(read_message)); //이거 해줘야 함
            if (read(client_socket,&read_message,sizeof(read_message))==-1){ //size -1?
                perror("read error");
                close(client_socket);
                exit(1);
            }
            if(i==1)
                ;
                //printf("read is %s\n\n\n\n\n",read_message);
             
            printf_read_message(read_message, num_command, &mode,&num_bulk,&len_num);
            if(mode!=0)
                mode=2;//2이면 위 함수에서 다른 것 실행
            if(i==(num_bulk-1)/MAX_BUFFER)
                mode=3;
            if(i==(num_bulk-1)/MAX_BUFFER+1)
                break;
            //printf("i is%d\n",i);
            i++;
        }
        if (close(client_socket)==-1){
            perror("close error");
            exit(1);
        }
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

void make_resp_form(char * send,int *first,char* come, int *num_command){
    char rn[5]="\r\n";
    char command[MAX_BUFFER];
    //memset(command,0,sizeof(command)); //하면 안됨
    char dollar[2]="$";
    char star[2]="*";
    char *end;
    if (!*(first))
        end=fgets(command,sizeof(command),stdin);
    while(end!=NULL && *num_command!=1) {
        char one_line[MAX_BUFFER]="";
        memset(one_line,0,sizeof(one_line));
        char copy[MAX_BUFFER];
        memset(copy,0,sizeof(copy));
        strcpy(copy, command);
        if (command[strlen(command)-1]=='\n')
            command[strlen(command)-1]='\0';
        if (strstr(command,"\"")!=NULL){
            char *quote =strtok(copy,"\"");
            quote =strtok(NULL,"\"");
            char *bulk = strtok(command," ");
            int num_bulk = 0; 
            char maker[MAX_BUFFER]="";
            memset(maker,0,sizeof(maker));
            while(bulk!=NULL){
                char len_bulk[100]="";
                memset(len_bulk,0,sizeof(len_bulk));
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

            char char_num_bulk[100]="";
            memset(char_num_bulk,0,sizeof(char_num_bulk));
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
            char maker[MAX_BUFFER]="";
            memset(maker,0,sizeof(maker));
            while(bulk!=NULL){
                char len_bulk[100]="";
                memset(len_bulk,0,sizeof(len_bulk));
                strcat(maker,dollar);
                sprintf(len_bulk,"%ld",strlen(bulk));
                strcat(maker,len_bulk);
                strcat(maker,rn);
                strcat(maker,bulk);
                strcat(maker,rn);
                num_bulk+=1;
                bulk = strtok(NULL," ");
            }

            char char_num_bulk[100]="";
            memset(char_num_bulk,0,sizeof(char_num_bulk));
            sprintf(char_num_bulk,"%d",num_bulk);
            strcat(one_line,star);
            strcat(one_line,char_num_bulk);
            strcat(one_line,rn);
            strcat(one_line,maker);

        }
        strcat(send,one_line);
        strcat(send,"\r\n");
        *num_command+=1;
        end=fgets(command,sizeof(command),stdin);
    }
    *first=1;
    if(end==NULL)
        *first=-1;
        return;
    strcpy(come,end);
    return ;
}


void printf_read_message(char *read_message,int num_command, int *mode,int *num_bulk,int *len_num){
    char *divide = strtok(read_message,"\r");
    
    if(*mode==2){
        int i;
        for(i=0;i<MAX_BUFFER ;i++){ //sizeof or maxsize or max_buffer
            if(*(read_message+i)=='\x00'){
                printf("\x0D");
                continue;
            }
            printf("%c",*(read_message+i));
        }
        return;
    }
    if(*mode==3){
        int i;
        for(i=0;i<*num_bulk%MAX_BUFFER+3+*len_num ;i++){ //sizeof or maxsize or max_buffer
            printf("%c",*(read_message+i));
        }
        printf("\n");
        return;
    }
    for(int i=0;i<num_command;i++){
        //printf("divide is %c and %p\n",*divide, divide);
        switch(*divide){
            case '+':
                printf("2222222222\n");
                printf("%s\n",divide+1);
                divide = strtok(NULL,"\r");
                divide+=1;
                break;
            case '-':
                printf("333333333\n");
                break;
            case ':':
                printf("11111111111\n");
                printf("%s\n",divide+1);
                divide = strtok(NULL,"\r");
                divide+=1;
                break;
            case '$':{
                //printf("hi\n");
                if( strncmp(divide+1,"-",1)==0){ ///null관련
                    printf("333333333\n");
                    printf("\0");   /////
                    printf("\n");   /////
                    divide = strtok(NULL,"\r");
                    divide+=1;
                    break;
                }
                int inum_bulk=atoi(divide+1);
                int length_num=strlen(divide+1);
                *num_bulk=inum_bulk;
                *len_num=length_num;
                for(int i=0;i<MAX_BUFFER-3-length_num;i++){
                    printf("%c",*(read_message+i+length_num+3));
                    //printf("i is %d",i);
                }
                divide+=1+length_num+inum_bulk;
                break;
            }
            case '*':
                printf("4444444\n");
                break;
            default :    
                printf("err\n");
                
        }
    }
}