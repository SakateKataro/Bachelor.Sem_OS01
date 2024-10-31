#include "user.h"

int main(int argc, char *argv[]) {
    int p1[2]; // Pipe for parent to child
    int p2[2]; // Pipe for child to parent
    char byte = 'A'; // The byte to ping-pong
    int pid;

    // Create pipes
    if (pipe(p1) < 0) {
        printf("pipe error\n");
        exit(1);
    }
    if (pipe(p2) < 0) {
        printf("pipe error\n");
        exit(1);
    }

    // Fork a new process
    pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        exit(1);
    }

    if (pid == 0) { // Child process
        close(p1[1]); // Close write end of p1
        close(p2[0]); // Close read end of p2

        // Read byte from parent
        read(p1[0], &byte, 1);
        printf("%d: received ping\n", getpid());

        // Send byte back to parent
        write(p2[1], &byte, 1);

        close(p1[0]); // Close read end of p1
        close(p2[1]); // Close write end of p2
        exit(0);
    } else { // Parent process
        close(p1[0]); // Close read end of p1
        close(p2[1]); // Close write end of p2

        // Send byte to child
        write(p1[1], &byte, 1);

        // Read byte back from child
        read(p2[0], &byte, 1);
        printf("%d: received pong\n", getpid());

        close(p1[1]); // Close write end of p1
        close(p2[0]); // Close read end of p2
        wait(0); // Wait for child to exit
    }

    exit(0);
}
