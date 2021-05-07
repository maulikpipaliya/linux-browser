// #include "global.h"

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "thread_controller.h"

void *thread_routine(void *args)
{
    printf("[INFO] : I am a new tab with URL %s with threadID: %u.\n", (char *)args, (unsigned int)pthread_self());

    char *URL = (char *)args;
    char file_name[30];
    sprintf(file_name, "%u.html", (unsigned int)pthread_self());

    /*
    
        URL to IP
    
    */

    char ip[50];
    hostname_to_ip(URL, ip);
    trim(ip);
    printf("%s resolved to %s\n", URL, ip);

    /*
        Connect to IPAddress
    */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Cant create socket");
        return 1;
    }

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    server_address.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error in connecting");
        return 1;
    }

    printf("Success : Page connected. PAGE CONTENT BELOW \n\n");
    printf("----------------------------------------------------\n");

    char sendline[500];

    strcpy(sendline, "GET /\r\n");
    write(sockfd, sendline, strlen(sendline));

    char rcvline[4096];

    int n;

    FILE *fp;

    fp = fopen(file_name, "w");

    while ((n = read(sockfd, rcvline, 4096)) > 0)
    {
        rcvline[n] = 0;
        fprintf(fp, "%s", rcvline);
    }
    fclose(fp);

    if (n < 0)
    {
        printf("Error in file");
    }

    // if (fork() == 0)
    // {
        char file[100];
        sprintf(file, "./link_finder.sh %u.html", (unsigned int)pthread_self());
        // sprintf(file, "./link_finder.sh %s.html", url);
        system(file);
        // execlp("./link_finder.sh ", "./link_finder.sh ", file);
    // }
    // wait(NULL);
    printf("----------------------------------------------------\n");
    pthread_exit(0);
}
