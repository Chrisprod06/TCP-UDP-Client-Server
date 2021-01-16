/*Christophoros Prodromou 14598 Vangelis Photiou 14585*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_USERS           10
#define USER_PASS_LENGTH    16
#define NETWORK_BUFFER      512
#define NETWORK_BUFFER      512
#define BACKLOG 	    5
#define LOCAL_PORT          0

int main (int argc, char *argv[]) {

    int sockfd, rc, i, serverPort, nBytes;
    struct sockaddr_in localAddr, servAddr;
    struct hostent *h;
    char option=' ';
    char fNf='0';
    char s;

    /* Check parameters from command line */

    if(argc != 3)//check number of argc in command line
    {
        fprintf(stderr,"Usage: tcpclient [IP_server] [server_port]\n");
        return -1;
    }

    serverPort = atoi(argv[2]);
    if(serverPort<=0 || serverPort>65535)//check number of TCP server port
    {
        fprintf(stderr, "The port number given is wrong.\n");
        return -1;
    }

    h = gethostbyname(argv[1]);
    if(h==NULL)//check assigment of TCP server host
    {
        perror("Unknown host ");
        return -1;
    }

    /* Create TCP socket */
    
    servAddr.sin_family = h->h_addrtype;
    memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr.sin_port = htons(serverPort);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)//check if TCP socket is created correctly
    {
        perror("Cannot open socket ");
        return -1;
    }

    /* Bind any port number */

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(0);

    rc = bind(sockfd, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if(rc < 0)//check TCP socket is bind correctly
    {
        perror("Cannot bind on TCP port ");
        close(sockfd);
        return -1;
    }

    /* Connect to TCP server */

    rc = connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if(rc < 0)//check TCP socket is connected correctly
    {
        perror("Cannot connect ");
        close(sockfd);
        return -1;
    }

    printf("Starting TCP client...\n");

    char buffer[BUFSIZ];

    /* Ask to the user about file name to be transferred to the TCP server */

    printf("Please enter the file name to be transferred to the server: ");
    bzero(buffer, BUFSIZ);//clean buffer
    scanf("%s", buffer);

    /* Send file name to the TCP server */

    nBytes = write(sockfd, buffer, strlen(buffer));
    if (nBytes < 0)
    {
         perror("Error writing to socket ");
         close(sockfd);
         return -1;
    }

    /* Read the TCP server response about the name of the file */

    nBytes = read(sockfd, &option, 1);
    if (nBytes < 0)
    {
         perror("Error reading from socket ");
         close(sockfd);
         return -1;
    }
    if (option == fNf){// if file has not been found
        do
        {
            printf("Do you want to send the file? [y] Yes  [n] No\n");
            do
            {
                option = getchar();
            }while (isspace(option));
            if (option == 'y')
            {
                nBytes = write(sockfd, "1", 1);//send confirmation for creating file
                if (nBytes < 0)
                {
                    perror("Error writing to socket");
                    return -1;
                }
            }else if (option == 'n')//cancel file creation
            {
                  return 0;
            }else{
                  printf("Answer wrong!");
            }
        }while((option != 'y')&&(option != 'n'));
    }else{// if file has been found
        do
        {
            printf("The file exists, what do you want to do?\n[o] Overwrite [a] Append [c] Cancel: \n");
            do
            {
                option = getchar();
            }while (isspace(option));
            if (option == 'o')
            {
               nBytes = write(sockfd,"1",1);//send confirmation for overwritting file
               if (nBytes < 0)
               {
                  perror("Error writing to socket");
                  close(sockfd);
                  return -1;
               }
            }else if (option == 'a')
            {
                  nBytes = write(sockfd,"1",1);//send confirmation for appending file
                  if (nBytes < 0)
                  {
                     perror("Error writing to socket");
                     close(sockfd);
                     return -1;
                  }
            }else if (option == 'c')
            {
                  return 0;
            }else if (option != '/0')//cancel file overwritting or appending
            {
                     printf("Answer wrong!");
            }
        }while((option != 'o')&&(option != 'a')&&(option != 'c'));
    }

    /* Read from file and send to TCP server*/

    FILE * fp;
    fp = fopen(buffer, "r");
    if(fp == NULL)//check file is open correctly
    {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }
    do
    {
        s = fgets(buffer, BUFSIZ, fp);//read from file
        if(s != NULL && buffer[0] != EOF)
            nBytes = write(sockfd, buffer, strlen(buffer));//send the information to TCP server
    }
    while(s != NULL);

    printf("File was successfully sent to the server.");

    /* Read the TCP server response about the transfer of the file and show*/

    bzero(buffer,BUFSIZ);//clean buffer
    nBytes = read( sockfd,buffer, BUFSIZ);
    if (nBytes < 0)
    {
        perror("Error reading from TCP socket");
        close(sockfd);
        return -1;
    }
    printf("%s\n",buffer);

    close(sockfd);
    return 0;
}

