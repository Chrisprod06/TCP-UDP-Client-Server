/*Christophoros Prodromou 14598 Vangelis Photiou 14585*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS           10
#define USER_PASS_LENGTH    16
#define NETWORK_BUFFER      512
#define NETWORK_BUFFER      512
#define BACKLOG 	    5
#define LOCAL_PORT          0

int main(int argc, char *argv[])
{

    if(argc != 3)
    {//Check if program is correclty called
        fprintf(stderr, "Usage: udpserver [usersfile] [server_port]\n");
        return -1;
    }
      char namepass[5];

    FILE *ptr;

    //read name and password from login file
    if((ptr = fopen("login.txt","r")) == NULL)
        printf("File could not be found");
    else
    {
            fscanf(ptr,"%s",namepass);
            fclose(ptr);
    }


    int serverPort = atoi(argv[2]);
    if(serverPort<=0 || serverPort>65535)
    {//Checking if the port given is valid and withing the accepted values
        fprintf(stderr, "The port number given is wrong.\n");
        return -1;
    }

    printf("Starting UDP server...\n");
    //Initializing the socket variables
    int sockfd, bytesReceived;
    struct sockaddr_in serverAddr,clientAddr;
    socklen_t len;
    char buffer[NETWORK_BUFFER];
    //Creating a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        perror("Cannot open socket ");
        return -1;
    }

    //Preparing serverAddr
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    //Binding on the socket
    if(bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Cannot bind on UDP port");
        close(sockfd);
        return -1;
    }
    fprintf(stderr, "Bind on UDP port %d.\n", serverPort);

    bool correctCredentials;

    while(true)
    {
        correctCredentials = false;
        //for(int i = 0; i < 3 && !correctCredentials; i++)
        //{
            len = sizeof(clientAddr);
	    //Listening from clients
            bytesReceived = recvfrom(sockfd, buffer, NETWORK_BUFFER, 0, (struct sockaddr *)&clientAddr, &len);
            correctCredentials = strcmp(buffer,namepass);
        //}
        //Check if the credentials provided are valid, send back an ACK/NACK message
        if(correctCredentials)
        {
            printf("User connected on the server.\n");
            sendto(sockfd, "^F", NETWORK_BUFFER, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        }
        else
        {
            printf("You have not provided valid credentials.\n");
            sendto(sockfd, "^U", NETWORK_BUFFER, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        }
    }

    //close the socket
    close(sockfd);
    return 0;
}










