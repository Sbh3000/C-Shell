#include "headers.h"
int check = 0;
char *Initial_DIR = "";
void seek(int argc, char *argv[])
{
    int dflag = 0, fflag = 0, eflag = 0;
    char *search = NULL;
    char *dir = ".";

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-f"))
            fflag = 1;
        else if (!strcmp(argv[i], "-d"))
            dflag = 1;
        else if (!strcmp(argv[i], "-e"))
            eflag = 1;
        else if (search == NULL)
        {
            search = argv[i];
            // printf("Search is %s",search);
        }
        else
        {
            dir = argv[i];
            Initial_DIR = argv[i];
            // printf("\nInitoal Dir is %s",Initial_DIR);
        }
    }
    if (!search || argc == 1)
    {
        printf("Please enter file or directory name\n");
        return;
    }
    if (dflag && fflag)
    {
        printf("Invalid flags!\n");
        return;
    }
    Seek_DIR(dir, search, dflag, fflag, eflag);
    if (check != 1)
    {
        printf("\nNone of the file or DIR Found\n");
    }
}

void Seek_DIR(char *dir_store, char *search, int dflag, int fflag, int eflag)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char fullpath[1024];
    int found = 0;
    int *p;

    if ((dir = opendir(dir_store)) == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        sprintf(fullpath, "%s/%s", dir_store, entry->d_name);
        if (stat(fullpath, &info) == -1)
        {
            perror("stat");
            continue;
        }
        if (S_ISDIR(info.st_mode))
        {
            if (!fflag && !strcmp(entry->d_name, search))
            {
                if (strlen(Initial_DIR) > 1)
                {
                    printf("%s\n", fullpath + strlen(Initial_DIR));
                }
                else
                {
                    printf("%s\n", fullpath);
                }
                check = 1;
                found++;
            }
            Seek_DIR(fullpath, search, dflag, fflag, eflag);
        }
        else
        {
            if (!dflag && !strcmp(entry->d_name, search))
            {
                if (strlen(Initial_DIR) > 1)
                {
                    printf("%s\n", fullpath + strlen(Initial_DIR));
                }
                else
                {
                    printf("%s\n", fullpath);
                }
                check = 1;
                found++;
            }
        }
    }

    closedir(dir);

    if (eflag && found == 1)
    {
        if (dflag)
        {
            char *A[2];
            A[0] = (char *)malloc((4 + 1) * sizeof(char));
            strcpy(A[0], "warp");
            A[1] = (char *)malloc((strlen(fullpath) + 1) * sizeof(char));
            strcpy(A[1], fullpath);
            warp(2, A);
            // printf("%s*\n", fullpath);
            free(A[0]);
            free(A[1]);
            check = 1;
            return;
        }
        else if (fflag)
        {
            FILE *fp = fopen(fullpath, "r");
            if (fp == NULL)
            {
                printf("Missing permissions for task!\n");
                return;
            }
            char c;
            while ((c = fgetc(fp)) != EOF)
                putchar(c);
            fclose(fp);
            check = 1;
        }
    }
}