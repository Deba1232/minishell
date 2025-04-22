#include "msh.h"

int status;

void execute_internal_commands(char *command){
    if(strcmp(command, "exit") == 0){
        printf("logout\n");
        exit(0);
    }
    else if(strcmp(command, "pwd") == 0){
        char pwd[50];
        if(!getcwd(pwd, sizeof(pwd))){
            perror("getcwd");
        }
        else{
            printf("%s\n", pwd);
        }
    }
    else if(strncmp(command, "cd", 2) == 0){
        if(strchr(command, ' ')){
            chdir(strchr(command, ' ') + 1);
        }
        else{
            chdir("/home");
        }
    }
    else if(strcmp(command, "echo $$") == 0){
        printf("%d\n", getpid());
    }
    else if(strcmp(command, "echo $?") == 0){
        printf("%d\n", WEXITSTATUS(status));
    }
    else if(strcmp(command, "echo $SHELL") == 0){
        printf("%s\n", getenv("SHELL"));
    }
    
}

void execute_external_commands(char *command){
    if(strchr(command, '|')){

    }
    else{
        // To execute an external command, we'll split the input command string into individual tokens (command and its arguments) and store them in a 1D array of strings.
        // This array will later be passed to execvp for execution.
        char *cmd_buf[50];
        
        int cmd_buf_idx = 0;

        char *token = strtok(command, " ");
        while(token != NULL){
            cmd_buf[cmd_buf_idx++] = token;
            token = strtok(NULL, " ");
        }

        // Null-terminate the array for execvp compatibility
        cmd_buf[cmd_buf_idx] = NULL;
        
        // Now create a child process in order to execute the command
        pid_t pid = fork();

        if(pid > 0){    // Parent process
            wait(&status);
        }
        else if(pid == 0){  // Child process
            execvp(cmd_buf[0], cmd_buf);
        }
    }
}