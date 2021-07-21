#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX 1000
#define PORT 8080
#define SA struct sockaddr
char *part[1000];
int parse(char *buffer)
{
    char *token = strtok(buffer, " ");
    int i = 0;
    while (token != NULL)
    {
        part[i] = token;
        //printf("%s\n", part[i]);
        i++;
        token = strtok(NULL, " ");
    }
    return i;
}
void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        char buffer[1000];

        printf("\n\nclient>");
        //scanf("%s", buffer);
        gets(buffer);
        int k = parse(buffer);
        // printf("number of arguments %d \n", k);
        int r = 0, s = 0;

        for (int i = 0; i < k; i++)
        {

            // bzero(buff, sizeof(buff));
            char *buff;
            buff = (char *)malloc(1000 * sizeof(buff));
            //printf("client>");
            n = 0;
            /*while ((buff[n++] = getchar()) != '\n')
                ;
            buff[n - 1] = '\0';*/
            strcpy(buff, part[i]);
            //printf("%s\n", buff);
            if (strcmp(buff, "exit") == 0)
            {
                exit(0);
            }
            if (i == 0)
            {
                if (strcmp(part[0], "get") != 0)
                {
                    printf("usage is get <file1> <file2> <file3>\n");
                    break;
                }
                else
                {
                    continue;
                }
            }
            write(sockfd, part[i], strlen(part[i]));
            s++;
            char *filename;
            filename = (char *)malloc(1000 * sizeof(char));
            strcpy(filename, buff);
            //free(buff);
            //buff = (char *)malloc( 1000 * sizeof(char));

            /*char *str;
            str = (char *) malloc(100 * sizeof(char));*/
            char str[100];

            read(sockfd, str, 100);
            r++;

            // perror("fsddfa : ");
            printf("this is of size %s", str);
            long long int k = atol(str);
            //free(str);
            // printf("%lld\n",k);
            if (k == -1)
            {

                printf("\nfrom server :  sorry i dont have  %s file :()\n\n\n", buff);
                continue;
            }
            if (k == 0)
            {
                int zero = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
                printf("\nDownload completed :)\n\n");
                continue;
            }
            // printf(" size of file is %lld\n", k);

            //bzero(buff, sizeof(buff));

            int fd1 = open(filename, O_RDWR | O_TRUNC);
            long long int total = 0;
            int fd3;

            printf("\nfile : %s\n", filename);

            while (1)
            {
                buff = (char *)malloc(1000 * sizeof(buff));

                int rec = read(sockfd, buff, 999);
                r++;

                // printf("recv at every instant %d\n", rec);

                //printf("From Server : %s", buff);

                //int fd1 = open(filename, O_RDWR );
                if (fd1 == -1)
                {
                    fd3 = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
                    // perror("creation: ");
                    fd1 = fd3;
                }
                else
                {
                    fd3 = fd1;
                }
                lseek(fd3, 0, SEEK_END);
                write(fd3, buff, rec);
                total += rec;
                long double x = (long double)(total * 100) / k;
                //char *msg = (char *)malloc(32 * sizeof(char));

                printf("\rpercentage downloaded : %0.2Lf", x);
                fflush(stdout);
                if (total == k)
                {
                    printf("\n");
                    break;
                }
                free(buff);
                //close(fd1);
            }
            printf("Download completed :)\n");
            printf("\n");
            /*if ((strncmp(buff, "exit", 4)) == 0)
            {
                printf("Client Exit...\n");
                break;
            }*/
        }
        // printf("reads %d writes %d\n", r, s);
        //sleep(1);
    }
}

int main()
{
    int sockfd;
    int connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
