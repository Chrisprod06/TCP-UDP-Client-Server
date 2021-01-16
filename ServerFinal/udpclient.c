/*Christophoros Prodromou 14598 Vangelis Photiou 14585*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_USERS           10
#define USER_PASS_LENGTH    16
#define NETWORK_BUFFER      512
#define NETWORK_BUFFER      512
#define BACKLOG 	    5
#define LOCAL_PORT          0


int main(int argc, char *argv[])
{

/* Check parameters from command line */

    if(argc != 3)//check number of argc in command line
    {
        fprintf(stderr,"Usage: udpclient [IP_server] [server_port]\n");
        return -1;
    }

    int serverPort = atoi(argv[2]);//check number of TCP server port
    if(serverPort<=0 || serverPort>65535)
    {
        fprintf(stderr, "The server port number given is wrong.\n");
        return -1;
    }

    int sockfd, bytesReceived;
    struct sockaddr_in serverAddr, clientAddr;
    struct hostent *hp;
    socklen_t len;

    /* Create UDP socket */

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)//check UDP socket is created correctly
    {
        perror("Cannot open socket ");
        return -1;
    }

    hp = gethostbyname(argv[1]);
    if (hp == 0)//check assigment of UDP server host
    {
        perror("Unknown Host ");
        close(sockfd);
        return -1;
    }
    
    /* Bind any port number */

    bzero((char *) &clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(LOCAL_PORT);

    if (bind(sockfd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)//check UDP socket is bind correctly
    {
        perror("Cannot bind ");
        close(sockfd);
        return -1;
    }


    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(serverPort);

    /* Insert credentials */
    
    bool correctCredentials = false;
    char username[USER_PASS_LENGTH], password[USER_PASS_LENGTH], buffer[NETWORK_BUFFER];
    for(int i = 0; i < 3 ;i++)//check number of tries to login
    {
        printf("Please enter username:");
        scanf("%s",&username);
        printf("Please enter password:");
        scanf("%s",&password);
        /* Prepare buffer to send*/
        strcpy(buffer, username);
        strcat(buffer, ":");
        strcat(buffer, password);
        //send credential request for confirming to UDP server
        sendto(sockfd, buffer, NETWORK_BUFFER, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        //read credential answer for confirming from UDP server
        bytesReceived = recvfrom(sockfd, buffer, NETWORK_BUFFER, 0, (struct sockaddr *)&clientAddr, &len);
        if(buffer[0] == '^' && buffer[1] == 'F')
        {
            correctCredentials = true;
            printf("You have connected successfully to the server.\n\n\n");
            char callFileServer[20] = "./tcpclient ";
            strcat(callFileServer, argv[1]);
            strcat(callFileServer, " 2424");
            system(callFileServer);
            close(sockfd);
            return 0;
        }
        else
            printf("The credentials you have provided are not valid.\n\n");
    }
    printf("You have provided three times wrong credentials.\n");

    close(sockfd);
    return 0;
}
