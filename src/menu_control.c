#include "global.h"
#include "menu_control.h"
int tab_count = 0;

int new_tab()
{
    char url[1000];
    printf("\n\nEnter an URL : ");
    scanf("%s", url);
    printf("%s\n\n", url);

    pthread_create(&window_tabs[tab_count], NULL, &thread_routine, (void *)&url);
    strcpy(tab_link[tab_count], url);

    tab_count++;

    sleep(4); // 
    return 0;
}

int active_tabs()
{
    int i = 0;
    printf("--------------ACTIVE TABS------------------\n\n");

    for (size_t i = 0; i < tab_count; i++)
    {
        printf("%d %s\n", (i + 1), tab_link[i]);
    }
    if (tab_count == 0)
    {
        printf("No active tabs \n");
    }

    printf("--------------------------------------------\n\n\n");
}

int goto_tab()
{
    active_tabs();
    printf("Select any one from active tabs : ");
    int c;
    scanf("%d", &c);

    // printf("%u",window_tabs[c-1]);

    char cmd[100];
    sprintf(cmd, "./link_finder.sh %u.html", (unsigned int)window_tabs[c - 1]);
    system(cmd);
}

int new_window()
{
    pid_t pgid = getpgid(0);
    char cmd[200];

    sprintf(cmd, "./main.out; exec bash");
    // system(cmd);
    pid_t child_window;

    if ((child_window = fork()) == 0)
    {
        execlp("gnome-terminal", "gnome-terminal", "-x", "bash", "-c", cmd, NULL);
        printf("-----forknewwindowison\n");
        exit(0);
    }

    printf("child window created %u\n", child_window);
    // system("gnome-terminal -x bash -c \"./main.out; exec bash\"");
    // if((pid==fork())==0){
    //     printf("[INFO] : I am a new window with pid = ", getpid());
    // }
    // system("gnome-terminal -x bash -c \"./main.out; exec bash\"");

    // window_tabs[++next_window][0] = pid;
}

int close_tab()
{
    active_tabs();
    printf("------CLOSE A TAB-----------------\n");
    printf("Select any one from active tabs : ");
    int c;
    scanf("%d", &c);

    char file_name[50];
    sprintf(file_name, "%u.html", (unsigned int)window_tabs[c - 1]);

    for (size_t i = c - 1; i < tab_count; i++)
    {
        window_tabs[i] = window_tabs[i + 1]; // shifting tabs backwards
        strcpy(tab_link[i], tab_link[i + 1]);
    }

    remove(file_name);
    tab_count--;
}

int close_window(int signal_code)
{
    FILE *fp = fopen("active_windows.txt", "r");
    FILE *inactive = fopen("inactive_windows.txt", "w");

    pid_t tpid, curr_pid = getpid();

    while (!feof(fp))
    {
        fscanf(fp, "%d\n", &tpid);

        if (tpid == curr_pid)
        {
            continue;
        }

        if (signal_code == -1)
        {
            kill(tpid, SIGKILL);
        }
        else
        {
            fprintf(inactive, "%d\n", tpid);
        }
    }

    fclose(fp);
    fclose(inactive);
    remove("active_windows.txt");
    rename("inactive_windows.txt", "active_windows.txt");

    if (signal_code == -1)
    {
        system("rm *.html *.pid.txt"); // unix
    }
    else
    {
        // Delete current window tabs only
        for (size_t i = 0; i < tab_count; i++)
        {
            char temp[20];
            sprintf(temp, "%u.html", window_tabs[i]);
            remove(temp);
            sprintf(temp, "%d.pid.txt", getpid());
            remove(temp);
        }
    }

    // system("exit");
    exit(0);
}

int active_windows()
{
    FILE *fp = fopen("active_windows.txt", "r");

    pid_t tpid_arr[MAX_WINDOWS];

    int i = 0;
    while (!feof(fp))
    {
        fscanf(fp, "%d\n", &tpid_arr[i]);

        printf("Window %d : %d\n", (i + 1), tpid_arr[i]);
        i++;
    }
    int which_window;
    printf("Show Tabs in which window ?\n");
    scanf("%d", &which_window);

    sleep(1);

    char temp[300];
    sprintf(temp, "%d.pid.txt", tpid_arr[which_window - 1]);

    int pipe_fd = open(temp, O_RDONLY);
    kill(tpid_arr[which_window - 1], SIGUSR1);
    // memset(temp, 0, sizeof(temp));

    // printf("%s\n",temp);

    read(pipe_fd, temp, 300);

    printf("%s\n", temp);
    char *tokens = strtok(temp, "|");

    char name[40];
    pthread_t thread_ids[MAX_TABS];
    i = 0;
    do
    {
        // printf("tokens  %d - %s\n", i, tokens);
        memset(name, 0, sizeof(name));
        if (strlen(tokens) < 5)
            break;
        sscanf(tokens, "%s - %u", name, &thread_ids[i++]);
        printf("%d %s\n", i, name);
    } while ((tokens = strtok(NULL, '|')) != NULL);

    printf("Enter your Choice:");
    int choice;
    scanf("%d", &choice);

    char cmd[100];
    sprintf(cmd, "./link_finder.sh %u.html", (unsigned int)thread_ids[choice - 1]);
    system(cmd);

    // printf("%s\n", temp);
}


void pipe_reader_handler()
{
    char temp[300], temp1[100];
    memset(temp, 0, sizeof(temp));
    for (size_t i = 0; i < tab_count; i++)
    {
        memset(temp1, 0, sizeof(temp1));
        sprintf(temp1, "%s - %u |", tab_link[i], window_tabs[i]);
        strcat(temp, temp1);
    }

    // lseek(fd, 0, SEEK_SET);
    // printf("temp - %s\n", temp);
    write(fd, temp, strlen(temp));
}

void *pipe_routine()
{
    char pipe_name[20];
    sprintf(pipe_name, "%d.pid.txt", getpid());
    mkfifo(pipe_name, 0777);
    fd = open(pipe_name, O_WRONLY);
}
