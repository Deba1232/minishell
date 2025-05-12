#include "msh.h"

char *external_commands[200];
int external_cmd_count;

void extract_external_commands(char **external_commands){
    char read_buf[1], cmd_buf[50];
    int buf_idx = 0;

    int fd = open("external_command.txt", O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Reading 1 byte
    while(read(fd, read_buf, 1) != 0){
        if(*read_buf == '\n'){
            cmd_buf[buf_idx] = '\0';

            // Store external command from the file to the array
            external_commands[external_cmd_count] = (char *)calloc(strlen(cmd_buf) + 1, sizeof(char));
            if(!external_commands[external_cmd_count]){
                perror("Memory couldn't be allocated\n");
                exit(EXIT_FAILURE);
            }
            strcpy(external_commands[external_cmd_count], cmd_buf);
            external_cmd_count++;

            // Reset the buffer index so that we get fresh data
            buf_idx = 0;
        }
        else{
            cmd_buf[buf_idx++] = *read_buf;
        }
    }

    // Capture last line if it doesn’t end with newline
    if(buf_idx > 0){
        cmd_buf[buf_idx] = '\0';

        external_commands[external_cmd_count] = (char *)calloc(strlen(cmd_buf) + 1, sizeof(char));
        if(!external_commands[external_cmd_count]){
            perror("Memory couldn't be allocated\n");
            exit(EXIT_FAILURE);
        }     
        strcpy(external_commands[external_cmd_count], cmd_buf);
        external_cmd_count++;
    }

    close(fd);
}

char *get_command(char *input_string){
    static char command[20];
    int idx = 0;

    for(int i = 0; input_string[i] != '\0'; i++){
        if(input_string[i] != ' '){
            command[idx++] = input_string[i];
        }
        else{
            break;
        }
    }

    command[idx] = '\0';

    return command;
}

int check_command_type(char *command){
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd","pushd", "popd", "dirs", "let", "eval", "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source", "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg", "bg"};

    for(int i = 0; i < sizeof(builtins) / sizeof(char *); i++){
        // Check whether the command is a builtin command or not
        if(strcmp(command, builtins[i]) == 0){
            return BUILTIN;
        }
    }
    
    for(int i = 0; i < external_cmd_count; i++){
        // Check whether the command is a external command or not
        if(strcmp(command, external_commands[i]) == 0){
            return EXTERNAL;
        }
    }

    if(command[0] == '\n'){
        return EMPTY_COMMAND;
    }

    return NO_COMMAND;
}