#include "headers.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


void neonate(int argc ,char * argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s -n [time_arg]\n", argv[0]);
        return;
    }

    int time_arg = atoi(argv[2]);
    if (time_arg <= 0) {
        fprintf(stderr, "time_arg must be a positive integer\n");
        return;
    }

    enableRawMode();

    pid_t pid;
    time_t start_time = time(NULL);

    while (1) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            printf("%d\n", pid);
            fflush(stdout);

            // Wait for time_arg seconds or until 'x' is pressed
            while (time(NULL) - start_time < time_arg) {
                char c;

                fd_set read_fds;
                FD_ZERO(&read_fds);
                FD_SET(STDIN_FILENO, &read_fds);
                struct timeval timeout;
                timeout.tv_sec = 0; // Don't wait for input
                timeout.tv_usec = 0;

                int select_result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);
                if (select_result == -1) {
                    perror("select");
                    exit(EXIT_FAILURE);
                } else if (select_result > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
                    if (read(STDIN_FILENO, &c, 1) == 1 && c == 'x') {
                        disableRawMode();
                        return;
                    }
                }
            }

            start_time = time(NULL);

            // Clean up any finished child processes
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }
    }
}
