#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include<errno.h> 

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h> 




#define buffsize 4096
#define MAXLINE 8192


int hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
    	herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
	
	return 1;
}

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

int main(int argc, char const *argv[])
{

    char *URL = argv[1];
    char *file_name = argv[2];

    /*
    
        URL to IP
    
    */

    char ip[50];
    hostname_to_ip(URL , ip);
    trim(ip);
	printf("%s resolved to %s\n" , URL , ip);


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

    printf("Success\n");

    char sendline[500];

    strcpy(sendline, "GET /\r\n");
    write(sockfd, sendline, strlen(sendline));

    char rcvline[buffsize];

    int n;

    FILE *fp;

    fp = fopen(file_name, "w");

    while ((n = read(sockfd, rcvline, buffsize)) > 0)
    {
        rcvline[n] = 0;
        fprintf(fp, "%s", rcvline);
    }
    fclose(fp);

    if (n < 0)
    {
        printf("Error in file");
    }


    system("./link_finder.sh");
    
    
    
    return 0;
}
