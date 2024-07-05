#include "headers.h"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

void prompt() {
    // Do not hardcode the prmopt
    char* host_name = malloc(1000*sizeof(char));
    char* user_name = malloc(1000*sizeof(char));
    CUR_DIR = malloc(2000*sizeof(char));
    char* output = malloc(4000*sizeof(char));
    getlogin_r(user_name,1000);
    gethostname(host_name,1000);
    getcwd(CUR_DIR,2000);
    strcpy(output,ANSI_COLOR_BLUE);
    strcat(output,user_name);
    strcat(output,ANSI_COLOR_RESET);
    char Atr1[] = "@";
    char Atr2[] = ":";
    char* Atr3 = "~";
    strcat(output,Atr1);  getlogin_r(user_name,1000);
    getcwd(CUR_DIR,2000); gethostname(host_name,1000);
    strcat(output,host_name);
    strcat(output,Atr2);
    if (strcmp(CUR_DIR,HOME_DIR) == 0)
    {
        strcat(output, Atr3);
    }
    else if (strlen(CUR_DIR) < strlen(HOME_DIR))
    {
        strcat(output,CUR_DIR);
    }
    else
    {   
        char *temp = strstr(CUR_DIR, HOME_DIR);
        if (temp != NULL) {
            *temp = '\0';
            strcat(output,Atr3);
            strcat(output,temp+strlen(HOME_DIR));
        } else {
            strcat(output,CUR_DIR);
        }
        
    }
    printf("%s",output);
    printf("%s"," :");
    free(host_name);
    free(user_name);
    free(output);
}
