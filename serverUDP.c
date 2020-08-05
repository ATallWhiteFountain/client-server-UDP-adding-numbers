#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if( argc < 2)
        handleErrors("init fault");

    // Datagram (UDP) socket configuration
    int serverSocket = socket( AF_INET, SOCK_DGRAM, 0 );
    if(serverSocket == -1) handleErrors("socket fault");

    struct sockaddr_in serverAdress;
    struct sockaddr_in clientAdress;
    memset(&serverAdress, 0, sizeof(serverAdress) );
    memset(&clientAdress, 0, sizeof(clientAdress) );
    socklen_t adrLength = sizeof(struct sockaddr_in);

    serverAdress.sin_family = AF_INET;
    int portRequested = atoi(argv[1]);
    serverAdress.sin_port = htons(portRequested);
    serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind( serverSocket, (struct sockaddr *) &serverAdress, adrLength ) == -1 )
        handleErrors("bind fault");

    // Preparing buffers for information exchange
    char message[64] = {'\0'};
    char buf[1024] = {'\0'};

    // Server loop
    while (1)
    {
        if ( recvfrom( serverSocket, buf, 1024, MSG_WAITALL, (struct sockaddr *) &clientAdress, &adrLength ) == -1 )
            handleErrors("recvfrom fault");

        if( !addUpNumbers(buf, message) )
            handleErrors("data format incorrect");

        if( sendto( serverSocket, message, 64, MSG_WAITALL, (struct sockaddr *) &clientAdress, adrLength ) == -1 )
            handleErrors("sendto fault");
    }

    return 0;
}