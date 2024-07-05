#include "headers.h"
char HOME_DIR[4000];
char *token[100];
int inBg[100];
char *CUR_DIR;
char *user_name;
char PREVWORK_DIR[4000];
char *History_List[15];
#define HISTORY_FILE "pastevents.txt"

struct bg_list *BG_LIST = NULL;

int main()
{
    // ignore ctrl-c, ctrl-z on shell because we want it to affect the foreground processes not the shell
    signal(SIGINT, SIG_IGN);
    // signal(SIGTTOU, SIG_IGN);
    // signal(SIGTTIN, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    // Keep accepting commands
    getcwd(HOME_DIR, 4000);
    strcpy(PREVWORK_DIR, HOME_DIR);
    for (int i = 0; i < 15; i++)
    {
        History_List[i] = (char *)malloc(1000 * sizeof(char));
        strcpy(History_List[i], "");
    }
    load_history();
    while (1)
    {
        struct bg_list *curr = BG_LIST;
        // Check if any background process has completed
        while (curr != NULL)
        {
            int status;
            if (strcmp(curr->status, "Completed") == 0)
            {
                curr = curr->next;
                continue;
            }
            pid_t result = waitpid(curr->pid, &status, WNOHANG | WUNTRACED);
            if (result > 0)
            {
                // The process is still running
                // curr->status = "Running";
                if (WIFSTOPPED(status))
                {
                    // The process has been stopped
                    curr->status = "Stopped";
                    printf("Sleep exited Abnormally (%d)\n",curr->command,curr->pid);
                }
                else if (WIFEXITED(status))
                {
                    // The process has completed
                    curr->status = "Completed";
                    printf("Sleep exited normally (%d)\n",curr->command,curr->pid);
                }
            }

            curr = curr->next;
        }
        // Print appropriate prompt with username, systemname and directory before accepting input

        prompt();

        char input[4096];
        if (fgets(input, 4096, stdin) == NULL)
        {
            exit(0); // or perform any cleanup needed before exiting
        }
        // fgets(input, 4096, stdin);
        input[strcspn(input, "\n")] = 0;
        int i = 0;
        if (strncmp(input, "pastevents", strlen("pastevents")) != 0)
        {
            Push_To_History(input);
            save_history();
        }
        int inputLen = strlen(input);
        int k = 0, tokenN = 0;
        char *tokens[100];
        char *st = input;
        for (int i = 0; i < inputLen; i++)
        {
            if (input[i] == '&' || input[i] == ';')
            {
                inBg[tokenN] = (input[i] == '&');
                token[tokenN++] = st;
                st = input + i + 1;
                input[i] = '\0';
            }
        }

        if (st < input + inputLen)
        {
            token[tokenN] = st;
            inBg[tokenN++] = 0;
        }

        for (int j = 0; j < tokenN; j++)
        {
            // Remove leading spaces and tabs
            while (isspace(*token[j]))
                token[j]++;
            // Remove trailing spaces and tabs
            char *end = token[j] + strlen(token[j]) - 1;
            while (end > token[j] && isspace(*end))
                end--;
            *(end + 1) = '\0';

            if (strchr(token[j], '|') != NULL)
            {
                // If it does, call your piping function
                piping(token[j], inBg[j]);
            }

            // Check if the token is not an empty string before executing
            if (strlen(token[j]) > 0)
            {
                Execute_Token(token[j], inBg[j]);
            }
        }
    }
}
