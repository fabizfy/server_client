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
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define MY_PORT_NUMBER 49999

int main(char argc, char *argv[])
{
    int listenfd;
    int option = 1;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);                              //creates the "waiter" for connection
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); //let's socket to be used again

    struct sockaddr_in servAddr;
    struct sockaddr_in clientAddr;

    memset(&servAddr, 0, sizeof(servAddr)); //allocates space

    //makes the server possible to be connected
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int binder;
    binder = bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr)); ///binds the socket to the address and port number

    if (binder < 0) //checks for binder error
    {
        perror("bind");
        exit(1);
    }

    listen(listenfd, 1); //waits for the client to make a connection

    int pid;
    int counter = 0;
    int connectfd = 0;
    int length = sizeof(struct sockaddr_in);

    while (1) //server stays up
    {
        // char *date;
        // struct hostent *hostEntry;
        // char *hostName;

        waitpid(-1, NULL, WNOHANG); //take cares of zombie process

        connectfd = accept(listenfd, (struct sockaddr *)&clientAddr, &length); //accept connection
        counter++;
        printf("connection received #%d.\n", counter);

        if (connectfd < 0) //checks if there is a connection
        {
            fprintf(stderr, "connection error %d\n", errno);
            exit(1);
        }

        if (pid = fork()) //multiple connection
        {

            //printf("pid = fork\n");
            close(connectfd);
        }

        else
        {
            //printf("else statement\n");
            close(listenfd);
            break;
        }
    }

    char *date;
    struct hostent *hostEntry;
    char *hostName;

    hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET); //gets the host entry

    if (hostEntry == NULL) //if there is no host
    {
        fprintf(stderr, "No host found %d\n", errno);
        exit(1);
    }

    hostName = hostEntry->h_name;              //gets the hostName
    printf("Connection from: %s\n", hostName); //prints out who connected

    //for some reason this doesn't work -_-'
    time_t curtime;
    date = ctime(&curtime);                   // gets current time
    send(connectfd, date, strlen((date)), 0); //sends to client

    // char Buff[256];
    // time_t curtime;
    // curtime = time(NULL); //To get the date
    // snprintf(Buff, sizeof(Buff), "%.24s\r\n", ctime(&curtime));
    // write(connectfd, Buff, strlen(Buff));

    return 0;
}
