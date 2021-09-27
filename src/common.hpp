#pragma once


void empty_function(){}
void make_resp_form(char * send,int *first,char* come, int *num_command);
void printf_read_message(char *read_message,int num_command);

void parse_and_call(char * send,char *line, int *len_len);
void function(char ** command,int num_bulk, char* send);

void PING(char **command,int num_bulk,char * send);
void GET(char **command,int num_bulk,char * send);
void SET(char **command,int num_bulk,char * send);
void STRLEN(char **command,int num_bulk,char * send);
void DEL(char **command,int num_bulk,char * send);
void EXISTS(char **command,int num_bulk,char * send);
void ERR(char **command,int num_bulk,char * send);