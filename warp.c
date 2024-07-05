#include "headers.h"
void warp(int argc, char *argv[])
{
    char *Final = malloc(5000 * sizeof(char));
    if (argc == 1)
    {
        strcpy(Final, HOME_DIR);
        if (chdir(Final) < 0)
        {
            printf("Error Changing DIR");
        }
        return;
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "~") == 0)
        {
            strcpy(Final, HOME_DIR);
        }
        else if (argv[i][0] == '~')
        {
            chdir(HOME_DIR);
            memmove(argv[i], argv[i] + 2, strlen(argv[i]));
            strcpy(Final, argv[i]);
        }
        else if (strcmp(argv[i], "-") == 0)
        {
            strcpy(Final, PREVWORK_DIR);
        }
        else
        {
            strcpy(Final, argv[i]);
        }

        if (chdir(Final) < 0)
        {
            printf("Error Changing DIR");
        }
        strcpy(PREVWORK_DIR, CUR_DIR);
        getcwd(CUR_DIR, 4000);
    }
}