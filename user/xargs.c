#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {
    
    // Prepare arguments for the command
    char *arguments[MAXARG];
    int i;
    for (i = 0; i < argc - 1; i++) {
        arguments[i] = argv[i + 1];
    }

    // Prepare buffer to read each line
    char buf[512];
    int n = 0;

    // Read from standard input line by line
    while (read(0, &buf[n], 1) == 1) {

        // Null-terminate the buffer
        if (buf[n] == '\n') {
            buf[n] = '\0';
            
            // Add the line from standard input as the last argument and null-terminate the arguments array
            arguments[i] = buf;
            arguments[i + 1] = 0;


            // Fork and execute the command with the new arguments        
            if (fork() == 0) {
                exec(arguments[0], arguments);
                // fprintf(2, "xargs: exec %s failed\n", arguments[0]);
                exit(0);

            } else {
                wait(0);
            }

            n = 0;
        } 
        else {
            n++;
        }
    }

    // Check for read error
    //if (n < 0) {
    //    fprintf(2, "xargs: read error\n");
    //    exit(1);
    //}

    exit(0);
}
