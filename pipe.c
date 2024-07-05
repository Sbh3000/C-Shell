#include "headers.h"

void piping(char *TOKEN, int inBg)
{
    // Split the token into separate commands based on the pipe character
    char *commands[100];
    int commandN = 0;
    commands[commandN] = strtok(TOKEN, "|");
    while (commands[commandN] != NULL)
    {
        commands[++commandN] = strtok(NULL, "|");
    }

    // Check for invalid use of pipe
    if (commandN == 0 || commands[0] == NULL || commands[commandN - 1] == NULL)
    {
        printf("Invalid use of pipe\n");
        return;
    }

    int fd[2];
    int in = 0;

    // Run all the commands sequentially from left to right
    for (int i = 0; i < commandN; i++)
    {
        pipe(fd); // Create a pipe

        if (fork() == 0)
        {
            dup2(in, STDIN_FILENO); // Replace stdin with the input end of the pipe
            if (commands[i + 1] != NULL)
            {
                dup2(fd[1], STDOUT_FILENO); // Replace stdout with the output end of the pipe
            }
            close(fd[0]); // Close the unused input end of the pipe

            Execute_Token(commands[i], inBg); // Execute the command

            exit(0);
        }
        else
        {
            wait(NULL);   // Wait for the child process to finish
            close(fd[1]); // Close the unused output end of the pipe
            in = fd[0];   // Save the input end of the pipe for the next command
        }
    }
}
