#include "headers.h"

void Print_Permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void Print_Dir_Entry(struct dirent *entry, char *dir_path)
{
    struct stat fileStat;
    char full_path[4000];
    snprintf(full_path, 4000, "%s/%s", dir_path, entry->d_name);

    if (stat(full_path, &fileStat) < 0)
        return;

    Print_Permissions(fileStat.st_mode);
    printf(" %lu ", (unsigned long)fileStat.st_nlink);
    printf("%s ", getpwuid(fileStat.st_uid)->pw_name);
    printf("%s ", getgrgid(fileStat.st_gid)->gr_name);
    printf("%lld ", (long long)fileStat.st_size);

    struct tm *tm_info;
    char date_str[50];
    tm_info = localtime(&fileStat.st_mtime);
    strftime(date_str, sizeof(date_str), "%b %d %H:%M", tm_info);

    printf("%s ", date_str);

    if (S_ISDIR(fileStat.st_mode))
        printf("\033[1;34m%s\033[0m\n", entry->d_name); // print directories in blue
    else if (fileStat.st_mode & S_IXUSR)
        printf("\033[1;32m%s\033[0m\n", entry->d_name); // print executables in green
    else
        printf("%s\n", entry->d_name); // print files in white
}

void Print_Dir(int f2, int f1, char *check_dir)
{
    struct dirent *de;
    DIR *dr = opendir(check_dir);
    if (dr == NULL)
    {
        printf("Could not open current directory");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] != '.' || f2)
        {
            if (f1)
            {
                Print_Dir_Entry(de, check_dir);
            }
            else
            {
                struct stat fileStat;
                char full_path[4000];
                snprintf(full_path, 4000, "%s/%s", check_dir, de->d_name);

                if (stat(full_path, &fileStat) < 0)
                    continue;

                if (S_ISDIR(fileStat.st_mode))
                    printf("\033[1;34m%s\033[0m\n", de->d_name); // print directories in blue
                else if (fileStat.st_mode & S_IXUSR)
                    printf("\033[1;32m%s\033[0m\n", de->d_name); // print executables in green
                else
                    printf("%s\n", de->d_name); // print files in white
            }
        }
    }

    closedir(dr);
}


void peek(int argc, char *argv[])
{
    if (argc == 1)
    {
        Print_Dir(0, 0, ".");
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            Print_Dir(1, 0, ".");
        }
        else if (strcmp(argv[1], "-l") == 0)
        {
            Print_Dir(0, 1, ".");
        }
        else if (strcmp(argv[1], "-la") == 0 || strcmp(argv[1], "-al") == 0)
        {
            Print_Dir(1, 1, ".");
        }
        else
        {
            Print_Dir(0, 0, argv[1]);
        }
    }
    if (argc == 3)
    {
        if (strcmp(argv[1], "-a") == 0 && strcmp(argv[2], "-a") == 0)
        {
            Print_Dir(1, 0, ".");
        }
        else if (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "-l") == 0)
        {
            Print_Dir(0, 1, ".");
        }
        else if (strcmp(argv[1], "-a") == 0 && strcmp(argv[2], "-l") == 0 || strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "-") == 0)
        {
            Print_Dir(1, 1, ".");
        }
        else if (strcmp(argv[1], "-a") == 0)
        {
            Print_Dir(1, 0, argv[2]);
        }
        else if (strcmp(argv[1], "-l") == 0)
        {
            Print_Dir(0, 1, argv[2]);
        }
        else if (strcmp(argv[1], "-la") == 0 || strcmp(argv[1], "-al") == 0)
        {
            Print_Dir(1, 1, argv[2]);
        }
        else
        {
            printf("Wrong Format");
        }
    }
    if (argc == 4)
    {
        if (strcmp(argv[1], "-a") == 0 && strcmp(argv[2], "-l") == 0 || strcmp(argv[1], "-l") == 0 && strcmp(argv[1], "-a") == 0)
        {
            Print_Dir(1, 1, argv[3]);
        }
        else if (strcmp(argv[1], "-a") == 0 && strcmp(argv[2], "-a") == 0)
        {
            Print_Dir(1, 0, argv[3]);
        }
        else if (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "-l") == 0)
        {
            Print_Dir(0, 1, argv[3]);
        }
        else
        {
            printf("Give Input of the form peek -a -l <DIR/Name> ,If giving 4 input");
        }
    }
}