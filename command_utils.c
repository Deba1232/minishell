#include "msh.h"

void execute_internal_commands(char *command){
    if(strcmp(command, "exit") == 0){
        // Free dynamically allocated memory before exiting
        for (int i = 0; i < external_cmd_count; i++) {
            free(external_commands[i]);
            external_commands[i] = NULL;
        }

        printf("logout\n");
        exit(EXIT_SUCCESS);
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
    else if(strcmp(command, "jobs") == 0){
        if(stopped_process_count != 0){
            print_stopped_processes(head, tail);
        }
    }
    else if(strcmp(command, "fg") == 0){
        if(stopped_process_count == 0){
            printf("fg: current: no such job\n");
        }
        else{
            // Print the command that is being resumed
            printf("%s\n", tail->stopped_command);
            // Send SIGCONT to resume the stopped process
            kill(tail->child_process_id, SIGCONT);
            // Wait for the resumed process to terminate or stop again
            waitpid(tail->child_process_id, &status, WUNTRACED);
            // Remove the process from the stopped process list
            remove_stopped_process(&head, &tail);
        }
    }
    else if(strcmp(command, "bg") == 0){
        if(stopped_process_count == 0){
            printf("bg: current: no such job\n");
        }
        else{
            // Print the command that is being resumed in background
            printf("[%d] %s &\n", stopped_process_count, tail->stopped_command);
            // Send SIGCONT to resume the stopped process
            kill(tail->child_process_id, SIGCONT);
            // Wait for the resumed process to terminate or stop again
            waitpid(tail->child_process_id, &status, WNOHANG);
            // Remove the process from the stopped process list
            remove_stopped_process(&head, &tail);
        }
    }
}

void execute_piped_external_commands(char **piped_cmd, int piped_cmd_arr_size, int pipe_count){
    // Backup the original STDIN and STDOUT file descriptors for restoration later
    int backup_stdin = dup(STDIN_FILENO);
    int backup_stdout = dup(STDOUT_FILENO);

    // Store the starting indices of each command in the piped command sequence
    int cmd_start_indices[pipe_count + 1];
    int cmd_start = 0, idx = 0;

    // Find the position of each pipe symbol ("|") and replace it with NULL for execvp compatibility
    for(int i = 0; i < piped_cmd_arr_size; i++){
        if(strcmp(piped_cmd[i], "|") == 0){
            cmd_start_indices[idx++] = cmd_start;
            piped_cmd[i] = NULL;    // Split the command list for execvp
            cmd_start = i + 1;      // Next command starts after the pipe
        }
    }
    
    cmd_start_indices[idx] = cmd_start; // Index of the last command

    int prev_pipe_read_end = -1;
    pid_t pid;

    // Loop through each command in the pipeline
    for(int i = 0; i <= pipe_count; i++){
        int pipefd[2];

        // Create a pipe unless it's the last command
        if(i < pipe_count && pipe(pipefd) == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();

        // Here, since everytime a new child process will be created, the current process must read from the pipe, and the previous process must write to the pipe until the last iteration
        if(pid > 0){
            if(prev_pipe_read_end != -1){
                close(prev_pipe_read_end);
            }

            if(i < pipe_count){
                close(pipefd[PIPE_WRITE_END]);
                prev_pipe_read_end = pipefd[PIPE_READ_END];
            }

            // Redirect STDIN to read end of pipe
            // dup2(prev_pipe_read_end, STDIN_FILENO);
        }
        else if(pid == 0){
            if(i > 0){
                dup2(prev_pipe_read_end, STDIN_FILENO);
                close(prev_pipe_read_end);
            }

            if(i < pipe_count){
                close(pipefd[PIPE_READ_END]);
                //Redirect STDOUT to write end of pipe
                dup2(pipefd[PIPE_WRITE_END], STDOUT_FILENO);
                close(pipefd[PIPE_WRITE_END]);
            }

            // Now execute the piped commands
            execvp(piped_cmd[cmd_start_indices[i]], piped_cmd + cmd_start_indices[i]);
        }
        else{
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all the child processes to terminate
    for(int i = 0; i <= pipe_count; i++){
        wait(NULL);
        // waitpid(pid, &status, WUNTRACED);
    }
    
    // Restore original STDIN and STDOUT
    dup2(backup_stdin, STDIN_FILENO);
    dup2(backup_stdout, STDOUT_FILENO);

    close(backup_stdin);
    close(backup_stdout);
}

void execute_external_commands(char *command){
    // Count '|' in the command
    int pipe_count = 0;

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

    for(int i = 0; i < cmd_buf_idx; i++){
        if(strcmp(cmd_buf[i], "|") == 0){
            pipe_count++;
        }
    }

    if(pipe_count == 0){
        execvp(cmd_buf[0], cmd_buf);
    }
    else{
        execute_piped_external_commands(cmd_buf, cmd_buf_idx, pipe_count);
    }
}