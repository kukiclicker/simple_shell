#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char command[102];
    // default shell name is ">"
    char* shell_name = argc >= 2 ? argv[1] : ">";

    //loop that will keep terminal running until "exit" command is parsed or
    //terminal encounters CTRL+D (EOF)
    while (1) {
        printf("%s ", shell_name);
        if (fgets(command, 100, stdin) == NULL) {
            // EOF signal received, exit program
            printf("\n");
            exit(0);
        }

        // Remove newline character from input
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            // Exit program if user enters "exit" command
            exit(0);
        }
        //Trim input to prevent buffer overflow
        command[102] = '\0';

        //creating tokens from input command
        char *args[20];
        char *token = strtok(command, " \n");
        int i = 0;
        while (token != NULL) {
            args[i] = token;
            i++;
            token = strtok(NULL, " \n");
        }
        //assigning last argument to NULL as requested
        args[i] = NULL;

        //calling system function fork() in order to create new child process
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execvp(args[0], args);

            // If execvp returns, there was an error
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            // Parent process
            int status;
            //waiting for child process to finish executing
            waitpid(pid, &status, 0);
            printf("Child process %d completed with status %d\n", pid, status);
        } else {
            // Fork failed
            perror("fork");
            exit(1);
        }
    }

    return 0;
}

