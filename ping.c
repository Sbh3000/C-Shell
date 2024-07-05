#include "headers.h"

void update_status(int pid, int signal_number) {
    struct bg_list* current = BG_LIST;
    while (current != NULL) {
        if (current->pid == pid) {
            switch (signal_number) {
                case SIGSTOP:
                    current->status = "Stopped";
                    break;
                case SIGCONT:
                    current->status = "Running";
                    break;
                case SIGKILL:
                case SIGTERM:
                    current->status = "Terminated";
                    break;
                default:
                    current->status = "Unknown";
            }
            printf("Updated status of process with pid %d to %s\n", pid, current->status);
            break;
        }
        current = current->next;
    }
}

void ping(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ping <pid> <signal_number>\n");
        return;
    }

    pid_t pid = atoi(argv[1]);
    int signal_number = atoi(argv[2]) % 32;

    // Check if the process exists 
    if (kill(pid, 0) == -1) {
        perror("No such process found");
        return;
    }

    // Send the signal
    if (kill(pid, signal_number) == -1) {
        perror("Failed to send signal");
        return;
    }

    printf("Sent signal %d to process with pid %d\n", signal_number, pid);

    // Update the status of the list
    update_status(pid, signal_number);
}