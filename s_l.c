#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX 1000
#define PORT 8080
#define SA struct sockaddr

void repeat();
// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;)
    {

        int r = 0, s = 0;
        // bzero(buff, MAX);

        char *name;
        name = (char *)malloc(1000 * sizeof(char));
        // read the message from client and copy it in buffer
        int rep = read(sockfd, name, 100);
        r++;
        if (rep <= 0)
        {
            repeat();
            return;
            //continue;
        }
        // printf("%saaaaa\n",name);

        // printf("what i read is %s \n",buff);

        int fd = open(name, O_RDONLY);
        //perror("faffa:");
        if (fd == -1)
        {
            printf("iam in here\n");
            long long int size = -1;
            char str[100];

            sprintf(str, "%lld", size);
            write(sockfd, str, 100);
            s++;
            //bzero(buff, MAX);
            n = 0;
            usleep(0.000005);
            // strcpy(buff, "sorry i dont have that file : )( \n");
            /* while ((buff[n++] = getchar()) != '\n') 
			;*/
            //printf("sorry i do not have %s \n",buff);

            //write(sockfd, buff, sizeof(buff));
            continue;
        }

        long long int size = lseek(fd, 0, SEEK_END);
        char str[100];
        int quan;
        //printf("size of file is %lld \n",size);

        sprintf(str, "%lld", size);
        write(sockfd, str, 100);
        s++;
        usleep(0.000005);

        lseek(fd, 0, SEEK_SET);

        while (size != 0)
        {
            bzero(buff, MAX);

            n = 0;
            int dec;
            dec = 999;
            if (size > dec)
            {
                read(fd, &buff, dec);
                n += 1000;
                size -= dec;
                quan = dec;
            }

            else
            {
                read(fd, &buff, size);
                n += size;
                quan = size;
                size = 0;
            }
            //    printf("to client    : %s\n", buff);
            write(sockfd, buff, quan);
            //printf("i wrote this much %d\n",quan);
            s++;

            // and send that buffer to client
        }
        // printf("read is %d write is %d \n",r ,s);
        /*char over[10];
        strcpy(over, "over");
        write(sockfd, over, sizeof(over));*/
        // print buffer which contains the client contents
        /*printf("From client: %s\t To client : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n') 
			; */

        // and send that buffer to client
        //write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}
int sockfd, connfd, len;
struct sockaddr_in servaddr, cli;
void repeat()
{
    printf("Waiting for new client\n");
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
    func(connfd);
}
// Driver function
int main()
{
    // int sockfd, connfd, len;
    //struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}