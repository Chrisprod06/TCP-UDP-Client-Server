/*Christophoros Prodromou 14598 Vangelis Photiou 14585*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_USERS           10
#define USER_PASS_LENGTH    16
#define NETWORK_BUFFER      512
#define NETWORK_BUFFER      512
#define BACKLOG 	        5
#define LOCAL_PORT          0

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, serverPort, clientLength, nBytes;;
    char buffer[BUFSIZ];
    char option=' ';
    int sent=0;
    struct sockaddr_in serverAddr, clientAddr;

/* Check parameters from command line */

    if(argc != 2){//check number of argc in command line
        fprintf(stderr,"Usage: tcpserver [server_port]\n");
        return -1;
    }

    printf("Starting TCP server...\n");

    serverPort = atoi(argv[1]);
    if(serverPort<=0 || serverPort>65535)//check number of TCP server port
    {
        fprintf(stderr, "The port number given is wrong.\n");
        return -1;
    }

    /* Create a TCP socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)//check TCP socket is created correctly
    {
        perror("Error opening TCP socket");
        return -1;
    }

    /* Initialize TCP socket structure */

    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    /* Bind the host address using bind() call */

    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)//check TCP socket is bind correctly
    {
        perror("Error on binding TCP socket");
        return -1;
    }

    /* Start listening for the clients, TCP server wait for the incoming TCP client connection */

    listen(sockfd, BACKLOG);
    clientLength = sizeof(clientAddr);

    /* Accept the connection from the TCP client */

    newsockfd = accept(sockfd, (struct sockaddr *) &clientAddr, (socklen_t *) &clientLength);
    if (newsockfd < 0)//check actual connection is created correctly
    {
        perror("Error on accepting actual TCP client");
        return -1;
    }

    /* Start communicating between TCP client and server */

    bzero(buffer,BUFSIZ);
    nBytes = read( newsockfd,buffer,BUFSIZ );
    if (nBytes < 0)
    {
        perror("Error reading from TCP socket");
        return -1;
    }
    printf("This is the file to transfer: %s\n",buffer);

    /* Searching the file or creating file */
    FILE * file;
    char sample[10];
    if (fopen(buffer, "r") != NULL)
    {
        nBytes = write(newsockfd,"1",1);
        if (nBytes < 0)
        {
            perror("Error writing to TCP socket");
            return -1;
        }
    }
    else
    {
        nBytes = write(newsockfd,"0",1);
        if (nBytes < 0)
        {
            perror("Error writing to TCP socket");
            return -1;
        }
    }


    nBytes = read( newsockfd, sample,1 );
    if (nBytes < 0)
    {
        perror("Error reading from TCP socket");
        return -1;
    }

    option = sample[0];
    char readBuffer[NETWORK_BUFFER];
    switch (option)
    {
    case '1': //write or overwrite file
        if ((file = fopen(buffer, "w")) == NULL)
        {
            fprintf(stderr, "Cannot write on file.");
            return -1;
        }
        break;
    case '0': //append file
        if ((file = fopen(buffer, "a+")) == NULL)
        {
            fprintf(stderr, "Cannot append to file.");
            return -1;
        }
    }

    while((nBytes = read(newsockfd, readBuffer, NETWORK_BUFFER)) > 0)
    {
        if(nBytes < 0)
        {
            fprintf(stderr, "Error while receiving the file.\n");
            return -1;
        }
        fprintf(file, readBuffer);
        fprintf(stderr, readBuffer);
        if(strcmp(readBuffer,""))
            break;
    }
    fclose(file);
    sent=1;


    /* Send response to the TCP client */
    if (sent)
    {
        nBytes = write(newsockfd,"Transfer completed and successful",33);
    }
    else
    {
        printf("Error writting the file in TCP server");
    }
    if (nBytes < 0)
    {
        perror("Error writing to TCP socket");
        return -1;
    }

    return 0;
}
