#include "headers.h"
char command[1000];
char path[1000];
char HISTORY_FILE[1000];
void save_history()
{
    snprintf(HISTORY_FILE, 1000, "%s/pastevents.txt", HOME_DIR);
    // printf("%s",HISTORY_FILE);
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (fp == NULL)
    {
        perror("Error opening history file");
        exit(1);
    }
    for (int i = 0; i < 15; i++)
    {
        if (strcmp(History_List[i], "") != 0)
        {
            fprintf(fp, "%s", History_List[i]);
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
    return;
}

void load_history()
{
    snprintf(HISTORY_FILE, 1000, "%s/pastevents.txt", HOME_DIR);
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (fp == NULL)
    {
        return;
    }
    char line[1000];
    int i = 0;
    while (fgets(line, sizeof(line), fp) != NULL && i < 15)
    {
        line[strcspn(line, "\n")] = '\0';
        strcpy(History_List[i++], line);
    }
    fclose(fp);
    return;
}
void Push_To_History(char *var)
{
    for (int i = 0; i < 15; i++)
    {
        if (strcmp(History_List[14 - i], "") == 0)
        {
            if (i != 0)
            {
                if (!strcmp(History_List[i - 1], var))
                {
                    return;
                }
                else
                {
                    strcpy(History_List[i], var);
                    return;
                }
            }
            else
            {
                strcpy(History_List[14], var);
                return;
            }
        }
        else
        {
            if (!strcmp(History_List[14], var))
            {
                return;
            }
            for (int j = 0; j < 14; j++)
            {
                strcpy(History_List[j], History_List[j + 1]);
            }
            strcpy(History_List[14], var);
            return;
        }
    }
}

void pastevents(int argc, char *argv[], char *Arr[])
{
    if (argc == 1)
    {
        for (int i = 14; i >= 0; i--)
        {
            printf("%s\n", Arr[i]);
        }
        return;
    }
    else if (argc == 2 && !strcmp(argv[1], "purge"))
    {
        for (int i = 0; i < 15; i++)
        {
            strcpy(History_List[i],"");
        }
        save_history();
        return;
    }
    else if (argc == 3 && !strcmp(argv[1],"execute"))
    {
        char input[1000];
        strcpy(input , History_List[atoi(argv[2])-1]);
        // printf("%d",atoi(argv[2]));
        // printf("%s",input);
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
            // Maintain a one space margin between words inside the token
            char *p = token[j];
            for (int j = 0; token[j]; j++)
            {
                char *p = token[j];
                char *q = p;
                while (*p)
                {
                    if (isspace(*p))
                    {
                        *q++ = ' ';
                        while (isspace(*++p))
                            ;
                    }
                    else
                    {
                        *q++ = *p++;
                    }
                }
                *q = '\0';
            }
            Execute_Token(token[j],inBg[j]);
            return;
        }
    }
}