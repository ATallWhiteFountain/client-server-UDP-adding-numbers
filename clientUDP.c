#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if( argc < 3)
        handleErrors("init fault");

    // Datagram (UDP) socket configuration
    int clientSocket = socket( AF_INET, SOCK_DGRAM, 0 );
    if(clientSocket == -1) handleErrors("socket fault"); 

    struct sockaddr_in obtainedAdress;
    struct sockaddr_in serverAdress;
    memset(&serverAdress, 0, sizeof(serverAdress) );
    memset(&obtainedAdress, 0, sizeof(obtainedAdress) );
    socklen_t adrLength = sizeof(serverAdress);
    
    serverAdress.sin_family = AF_INET;
    int portRequested = atoi(argv[2]);
    serverAdress.sin_port = htons(portRequested);
    serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);

    // Response timeout configuration
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    int timeoutFd = setsockopt( clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout) );
    if( timeoutFd == -1 ) handleErrors("setsockopt fault");

    // Preparing buffers for information exchange
    char message[] = "256 128 64 32 16 8 4 2 1 0\r\n";
    int msgSize = strlen(message) + 1;
    char buf[64] = {'\0'};

    // Communication with server
    if( sendto( clientSocket, message, msgSize, MSG_WAITALL, (struct sockaddr *) &serverAdress, adrLength ) == -1 )
        handleErrors("sendto fault");

    if( recvfrom( clientSocket,(char *) buf, 64, MSG_WAITALL, (struct sockaddr *) &obtainedAdress, &adrLength ) == -1 )
        handleErrors("recvfrom fault");

    if( isLegal(buf) ) printf( "Feedback from server - sum of numbers: %s\n", buf );
    else printf("Illegal symbols found\n");

    if( close(clientSocket) == -1 ) handleErrors("close fault");

    return 0;
}