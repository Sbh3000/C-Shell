#include "headers.h"

void bg(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: bg <pid>\n");
        return;
    }
    else
    {
        int pid = atoi(argv[1]);
        struct bg_list *curr = BG_LIST;

        while (curr != NULL)
        {
            if (curr->pid == pid)
            {
                printf("%s",curr->status);
                if (strcmp(curr->status, "Stopped") == 0)
                {
                    kill(pid, SIGCONT);
                    curr->status = "Running";
                    printf("Changes [%d] %s's state to Running (in the background).\n", pid, *(curr->command));
                }
                return;
            }
            curr = curr->next;
        }

        printf("No such process found\n");
    }
       
}