#include "headers.h"

struct bg_list *head = NULL; // Initialize head to NULL

void activities(int argc, char *argv[])
{
    struct bg_list *current = BG_LIST;

    if (current == NULL)
    {
        printf("There exist no Activities\n");
        return;
    }
    while (current != NULL)
    {
        if (strcmp(current->status ,"Running") == 0|| strcmp(current->status , "Stopped") == 0 || strcmp(current->status , "Unknown") == 0)
        {
            printf("PID: %d, Status: %s\n", current->pid, current->status);
            current = current->next;
        }
        else
        {
            current = current->next;
        }
    }
}
