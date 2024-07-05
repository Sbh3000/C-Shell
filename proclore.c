#include "headers.h"
#include <string.h>

void proclore(int argc, char *argv[])
{
    char *p_id = malloc(1000 * sizeof(char));
    char *process_status = malloc(1000 * sizeof(char));
    char *process_group = malloc(1000 * sizeof(char));
    char *virtual_memory = malloc(1000 * sizeof(char));
    char *exec_path = malloc(2000 * sizeof(char));
    char *location = malloc(6000 * sizeof(char));
    char *output = (char *)malloc(1024 * sizeof(char));
    char *test = malloc(1000 * sizeof(char));
    strcpy(location, "");
    strcpy(output, "");
    strcpy(test, "~");
    if (argc == 1)
    {
        sprintf(p_id, "%d", getpid());
    }
    else
    {
        strcpy(p_id, argv[1]);
    }
    printf("pid : %s\n", p_id);

    sprintf(location, "/proc/%s/stat", p_id);
    FILE *f = fopen(location, "r");
    if (f == NULL)
    {
        perror("File not Found");
        return;
    }

    int i = 1;
    int tg_pid;

    fgets(output, 1024, f);

    char *token = strtok(output, " \t\n");

    while (token != NULL)
    {
        switch (i)
        {
        case 3:
            printf("process status : %s\n", token);
            break;
        case 5:
            printf("Process Group : %s", token);
            break;
        case 8:
            tg_pid = atoi(token);
            if (atoi(process_group) == tg_pid)
                printf("+");
            printf("\n");
            break;
        case 23:
            printf("Virtual memory : %s\n", token);
            break;
        }
        i++;
        token = strtok(NULL, " \t\n");
    }

    fclose(f);

    sprintf(location, "/proc/%s/exe", p_id);

    ssize_t len;

    if ((len = readlink(location, exec_path, 2000 - 1)) != -1)
        exec_path[len] = '\0';

   if (strcmp(exec_path,HOME_DIR) == 0)
   {
       free(exec_path);
       exec_path = strdup("~");
   }
   else if (strlen(exec_path) > strlen(HOME_DIR))
   {   
       char *temp = strstr(exec_path, HOME_DIR);
       if (temp != NULL) {
           exec_path[temp - exec_path] = '\0';
           strcat(exec_path,"~");
           strcat(exec_path,temp+strlen(HOME_DIR));
       } else {
           strcat(output,CUR_DIR);
       }
   }
   printf("executable path : %s\n", exec_path);

   free(p_id);
   free(process_status);
   free(process_group);
   free(virtual_memory);
   free(exec_path);
   free(location);
}
