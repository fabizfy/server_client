/*
Name: Fabiane Yeung
Instructor: Ben McCamish
CS 360 - Assignment 9
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define MY_PORT_NUMBER 49999

int main(char argc, char *argv[])
{
    int socketfd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0); //creates the socket (domain, type, protocol)

    struct sockaddr_in servAddr;
    struct hostent *hostEntry;
    struct in_addr **pptr;

    memset(&servAddr, 0, sizeof(servAddr));

    //needed to make the connection
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);

    hostEntry = gethostbyname(argv[1]); //connects to the server

    pptr = (struct in_addr **)hostEntry->h_addr_list; //writes hostEntry to the h_addr_list

    memcpy(&servAddr.sin_addr, *pptr, sizeof(struct in_addr)); //copy in_addr bytes to servAdd.sin_addr

    int con;
    con = connect(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr)); //connection

    if (con < 0)
    {
        fprintf(stderr, "connection error %d\n", errno);
        exit(1);
    }

    int valread;
    char buffer[256] = {0};

    valread = read(socketfd, buffer, 256);         //reads what was sent from server
    printf("receiving from server: %s\n", buffer); //prints what was received

    return 0;
}
