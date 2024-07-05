#include "headers.h"

void fg(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: fg <pid>\n");
        return;
    }

    int pid = atoi(argv[1]);
    struct bg_list *curr = BG_LIST;
    struct bg_list *prev = NULL;

    while (curr != NULL)
    {
        if (curr->pid == pid)
        {
            if (kill(pid, SIGCONT) == -1)
            {
                perror("kill");
                return;
            }

            // Wait for the process to finish
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("waitpid");
                return;
            }

            // Update the status
            if (WIFEXITED(status))
            {
                curr->status = "Completed";
                printf("%s [%d] has completed.\n", *(curr->command), pid);
            }
            else
            {
                curr->status = "Running";
                printf("Brings %s [%d] to foreground and change it's state to Running.\n", *(curr->command), pid);
            }

            // Remove the process from the background list
            if (prev == NULL)
            {
                BG_LIST = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            
            free(curr);
            
            return;
        }

        prev = curr;
        curr = curr->next;
    }

    printf("No such process found\n");
}
