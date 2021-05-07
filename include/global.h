#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include "util.h"
#include "network_util.h"
#include "thread_controller.h"

#define buffsize 4096
#define MAXLINE 8192

#define MAX_WINDOWS 10
#define MAX_TABS 15

typedef unsigned long int wt_t;

int fd;
pthread_t window_tabs[MAX_TABS];
pthread_t active_tab;


char tab_link[MAX_TABS][50];
