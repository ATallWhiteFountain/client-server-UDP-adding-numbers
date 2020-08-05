#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

bool isLegal(const char *buf){
    const char *ptr;
    ptr = buf;
    while(*ptr != 0){
        
        if( (*ptr < '/' || *ptr > ':' ) && *ptr != ' ' && *ptr != '\n' && *ptr != '\r' ) return false;
        ptr++;
    }
    return true;
}

bool isPrintable(const char *buf){
    const char *ptr;
    ptr = buf;
    while(*ptr != 0){
        
        if( isprint(*ptr) == 0  && *ptr != '\n' && *ptr != '\r' ) return false;
        ptr++;
    }
    return true;
}

bool addUpNumbers(char input[], char output[] ){

    if( !isLegal( (const char*) input ) ){
        strcpy((char *)output, "ERROR");
        return false;
    }
    else{
        char *liczby[16];
        char* seg = strtok(input, " ");
        liczby[0] = seg;
        int i = 0;
        while( seg != NULL){
            liczby[i] = seg;
            i++;
            seg = strtok(NULL, " ");
        }
        int j = 0, sum = 0;
        while( j < i ){
            sum += atoi(liczby[j]);
            j++;
        }

        if( sprintf(output, "%d", sum) < 0 ) printf("Buffer overfilled\n");
        strcat(output,"\r\n");
        return true;
    }
}

void handleErrors(const char* msg){
    printf("%s\n", msg);
    exit(1);
}