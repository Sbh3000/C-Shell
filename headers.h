#ifndef HEADERS_H_
#define HEADERS_H_

#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>

struct bg_list
{
    int index;
    int pid;
    char ** command;
    char * status;
    struct bg_list * next;
};

extern char HOME_DIR[4000];
extern char PREVWORK_DIR[4000];
extern char *token[100];
extern char *CUR_DIR;
extern char *user_name;
extern char *History_List[15];
extern int  inBg[100];
extern struct bg_list* BG_LIST;

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "proclore.h"
#include "seek.h"
#include "pastevents.h"
#include "execute.h"
#include "iMan.h"
#include "activities.h"
#include "functions.h"
#include "bg.h"
#include "fg.h"
#include "neonate.h"
#include "ping.h"
#include "pipe.h"
#endif