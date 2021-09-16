#include <stdio.h>
#include <string.h>
int index(char * str, char c);

int main(){
    char a[]="hellloo\r\n";
    int count;
    count = index(a,'\r');
    char b[]="hihihi";
    char c[]="dlafk";
    printf("%d\n",count);
    printf("%.*s\n",count,a);
    printf("a is %s", a);
}

int index(char * str, char c){
    for(int i=0;i<strlen(str);i++){
        if (*(str+i) == c ){
            return i;
        }
    }
}