#include "headers.h"

int bg_index = 0;

void Execute_Token(char *TOKEN, int inBg)
{
    int argc = 0;
    char *argv[100];
    argv[argc] = strtok(TOKEN, " ");
    while (argv[argc] != NULL)
    {
        argv[++argc] = strtok(NULL, " ");
    }

    int in = 0, out = 0;
    char *inputFile = NULL;
    char *outputFile = NULL;

    for (int j = 0; j < argc; j++)
    {
        if (strcmp(argv[j], "<") == 0)
        {
            inputFile = argv[j + 1];
            in = 1;
            argv[j] = NULL;
        }
        else if (strcmp(argv[j], ">") == 0)
        {
            outputFile = argv[j + 1];
            out = 1;
            argv[j] = NULL;
        }
        else if (strcmp(argv[j], ">>") == 0)
        {
            outputFile = argv[j + 1];
            out = 2;
            argv[j] = NULL;
        }
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        // error handling
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (in)
        {
            int fd0 = open(inputFile, O_RDONLY);
            if (fd0 < 0)
            {
                printf("No such input file found!\n");
                exit(1);
            }
            dup2(fd0, STDIN_FILENO);
            close(fd0);
        }
        if (out)
        {
            int fd1;
            if (out == 1)
                fd1 = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else
                fd1 = open(outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
        }

        if (!strcmp(argv[0], "warp"))
        {
            warp(argc, argv);
        }
        else if (!strcmp(argv[0], "peek"))
        {
            peek(argc, argv);
        }
        else if (!strcmp(argv[0], "proclore"))
        {
            proclore(argc, argv);
        }
        else if (!strcmp(argv[0], "seek"))
        {
            seek(argc, argv);
        }
        else if (!strcmp(argv[0], "pastevents"))
        {
            pastevents(argc, argv, History_List);
        }
        else if (!strcmp(argv[0], "iMan"))
        {
            iMan(argc, argv);
        }
        else if (!strcmp(argv[0], "activities"))
        {
            activities(argc, argv);
        }
        else if (!strcmp(argv[0], "bg"))
        {
            bg(argc, argv);
        }
        else if (!strcmp(argv[0], "fg"))
        {
            fg(argc, argv);
        }
        else if (!strcmp(argv[0], "neonate"))
        {
            neonate(argc, argv);
        }
        else if (!strcmp("ping", argv[0]))
        {
            ping(argc, argv);
        }
        else
        {
            // child process
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        }
        // if execvp returns, there was an error
        // Add this line
        exit(0);
    }
    else
    {
        if (!inBg)
        {
            int status = 0;
            waitpid(pid, &status, WUNTRACED);
            if (WIFSTOPPED(status))
            {
                printf("[%d] : %d\n", bg_index + 1, pid);
                BG_LIST = add(BG_LIST, bg_index++, pid, argv);
                struct bg_list *curr = BG_LIST;
                while (curr->next != NULL)
                {
                    BG_LIST = BG_LIST->next;
                }
                BG_LIST->status = "Stopped";
            }
        }
    }

    if (inBg)
    {
        printf(" [%d] : %d\n", bg_index + 1, pid);
        BG_LIST = add(BG_LIST, bg_index++, pid, argv);
    }
}