#include "global.h"
#include "menu_control.h"

int user_choice()
{
    int choice;
    while (1)
    {
        printf("\n");
        printf("1. New Tab\n");
        printf("2. Active Tabs\n");
        printf("3. Close Tab\n");
        printf("4. Go to Tab\n");
        printf("5. New Window\n");
        printf("6. Close Window\n");
        printf("7. Active Windows\n");
        // printf("99. Back to menu\n");
        printf("0. Exit everything, I dont wanna live anymore!\n");

        printf("Choose one : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            new_tab();
            break;

        case 2:
            active_tabs();
            break;

        case 3:
            close_tab();
            break;
        case 4:
            goto_tab();
            break;
        case 5:
            new_window();
            break;
        case 6:
            close_window(0);
            break;
        case 7:
            active_windows();
            break;
        case 0:
            close_window(-1);
            break;

        default:
            exit(1);
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, close_window);

    printf("Current Window ID : %d\n\n", getpid());
    FILE *fp = fopen("active_windows.txt", "a");
    fprintf(fp, "%d\n", getpid());
    fclose(fp);

    signal(SIGUSR1, pipe_reader_handler);

    pthread_t pipe_thread;
    pthread_create(&pipe_thread, NULL, pipe_routine, NULL);

    // int opt;

    // while ((opt = getopt(argc, argv, "n:")) != -1)
    // {
    //     switch (opt)
    //     {
    //     case 'n':
    //         printf("new window: %s\n", optarg);
    //         setpgid(0, atoi(optarg)); // not working
    //         // printf("%d\n",x);
    //         break;
    //     }
    // }

    // printf("pid-%u\n", getpid());
    // printf("pgid%u\n", getpgid(0));

    user_choice();

    // window_tabs[0] = getpid();
    return 0;
}
