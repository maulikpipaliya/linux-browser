#include <stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    FILE *in_file;

    in_file = fopen("info.html", "r");

    if (in_file == NULL)
    {
        printf("Error file missing\n");
        exit(-1);
    }

    int count = 0;

    while (fscanf(in_file, "%s", "a href=") == 1)
    {
        count++;
    }

    printf("%d = count\n", count);
    fclose(in_file);
    

    return 0;
}
