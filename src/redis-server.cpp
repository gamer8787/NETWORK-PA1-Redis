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
#include <map>
#include <iostream>
#include <string>
using namespace std;

map<string, string> m;
#define MAX_BUFFER 20000
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
    //////////////////////////////////////////////////
    while(1){
        client_socket = accept(server_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(client_socket == -1){
            perror("accept error");
            close(server_socket);
            }
        char read[MAX_BUFFER];   
        memset(read, 0x00, sizeof(read)); 
        if (recv(client_socket,read,sizeof(read),0)==-1){ //size -1?
            perror("read error");
            close(server_socket);
            exit(1);
        }
        char send[MAX_BUFFER]="";
        parse_and_call(send,read);
        printf("send is %s\n",send);
        if (write(client_socket,send,strlen(send))==-1){
            perror("write error");
            close(server_socket);
            exit(1); 
        }
        close(client_socket);
    }
    close(server_socket);

    return 0;  
}





void parse_and_call(char* send, char * line){
    char *token=strtok(line,"\r");;
    int num_bulk=atoi(token+1);
    char *command[num_bulk];
    for(int i=0;i<2*num_bulk;i++){
        token=strtok(NULL,"\r");
        if(i%2==1)
            command[(i-1)/2]=token+1;
    }
    function(command,num_bulk,send);
}

void function(char ** command,int num_bulk, char* send){
    if ((strncmp(command[0],"PING",4)==0 || strncmp(command[0],"ping",4)==0))
        PING(command,num_bulk,send);
    else if((strncmp(command[0],"GET",3)==0 || strncmp(command[0],"get",3)==0))
        GET(command,num_bulk,send);
    else if((strncmp(command[0],"SET",3)==0 || strncmp(command[0],"set",3)==0))
        SET(command,num_bulk,send);
    else if((strncmp(command[0],"STRLEN",6)==0 || strncmp(command[0],"strlen",6)==0))
        STRLEN(command,num_bulk,send);
    else if((strncmp(command[0],"DEL",3)==0 || strncmp(command[0],"del",3)==0))
        DEL(command,num_bulk,send);
    else if((strncmp(command[0],"EXISTS",6)==0 || strncmp(command[0],"exists",6)==0))
        EXISTS(command,num_bulk,send);
    else
        ERR(command,num_bulk,send);
        
}

/////////////////////////////////////////////////////////

void PING(char **command,int num_bulk,char * send){
    if(num_bulk==1) //ping하고 space 있을때도 생각?
        strcat(send,"+PONG\r\n");
    else if(num_bulk==2) {
        char len_bulk[100]="";
        sprintf(len_bulk,"%ld",strlen(command[1]));
        strcat(send,"$");
        strcat(send,len_bulk);
        strcat(send,"\r\n");
        strcat(send,command[1]);
        strcat(send,"\r\n");
    }
    else
        strcat(send,"-ERR\r\n");
}

void GET(char **command,int num_bulk,char * send){
    if (m.find(command[1]) == m.end()) {
        strcpy(send,"$-1\r\n");
        return;
    }
    char temp[MAX_BUFFER]="";
    string str(command[1]);
    strcpy(  temp , m[str].c_str()  );
    char len_bulk[100]="";
    sprintf(len_bulk,"%ld",strlen(temp));
    strcat(send,"$");
    strcat(send,len_bulk);
    strcat(send,"\r\n");
    strcat(send,temp);
    strcat(send,"\r\n");
}

void SET(char **command,int num_bulk,char * send){
    m.erase(command[1]);
    m.insert(pair<string, string>(command[1], command[2]));
    strcat(send,"+OK\r\n");
}

void STRLEN(char **command,int num_bulk,char * send){
    char temp[MAX_BUFFER]="";
    string str(command[1]);
    strcpy(  temp , m[str].c_str()  );
    char len_bulk[100]="";
    sprintf(len_bulk,"%ld",strlen(temp));
    strcat(send,":");
    strcat(send,len_bulk);
    strcat(send,"\r\n");
}

void DEL(char **command,int num_bulk,char * send){
    int num=0;
    for(int i=1;i<num_bulk;i++){
        if (m.find(command[i]) != m.end()) {
            num+=1;
            m.erase(command[i]);
        }
    }
    char num_del[100]="";
    sprintf(num_del,"%d",num);
    strcat(send,":");
    strcat(send,num_del);
    strcat(send,"\r\n");
}

void EXISTS(char **command,int num_bulk,char * send){
    int num=0;
    for(int i=1;i<num_bulk;i++){
        if (m.find(command[i]) != m.end()) {
            num+=1;
        }
    }
    char num_del[100]="";
    sprintf(num_del,"%d",num);
    strcat(send,":");
    strcat(send,num_del);
    strcat(send,"\r\n");
    
}

void ERR(char **command,int num_bulk,char * send){
    strcat(send,"-ERR\r\n");
}

//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