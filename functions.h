#ifndef __FUNC_H
#define __FUNC_H

void handle_eof(int sig);
void handle_sigint(int sig);
struct bg_list* init(int index, int pid, char** command);
struct bg_list* add(struct bg_list* head, int index, int pid, char** command);
struct bg_list* delete(struct bg_list* head, int index);

#endif