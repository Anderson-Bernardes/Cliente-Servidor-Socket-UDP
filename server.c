// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main()
{
    int sockfd;
    char buffer[MAXLINE];

    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    unsigned int len, n;
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                 &len);
    buffer[n] = '\0';


    DIR* diretorio;
    struct dirent* item_diretorio;
    diretorio = opendir( "./" );
    if( !diretorio )
    {
        perror( "opendir" );
        return( -1 );
    }
    item_diretorio = readdir( diretorio );
    while( item_diretorio )
    {

        sendto(sockfd, (const char *)item_diretorio->d_name, strlen(item_diretorio->d_name), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        printf( "Item = %s\n", item_diretorio->d_name );
        item_diretorio = readdir( diretorio );
    }
    sendto(sockfd, (const char *)"", strlen(""), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    closedir( diretorio );

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 0, ( struct sockaddr *) &cliaddr,
                 &len);
    buffer[n] = '\0';
    printf("O arquivo escolhido foi %s\n", buffer);

    FILE *arquivo;
    unsigned char leitura[2];

    arquivo=fopen(buffer, "rb");

    if(arquivo==NULL)
        printf("Errou");

    while(!feof(arquivo))
    {
        fread(&leitura, sizeof(char), 1, arquivo);
        sendto(sockfd, (const char *)leitura, sizeof(char), 0, (const struct sockaddr *) &cliaddr, len);
        printf("%s", leitura);
    }
    sendto(sockfd, (const char *)"", strlen(""), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

    return 0;
}
