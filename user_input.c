#include "msh.h"

void scan_input(char *prompt, char *input_string){
    // Store the external command list in an array
    extract_external_commands(external_commands);

    while(1){
        char pwd_buf[100], host_name_buf[50];
        gethostname(host_name_buf, sizeof(host_name_buf));

        printf(ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET ":" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "$ ", prompt, host_name_buf,getcwd(pwd_buf, sizeof(pwd_buf)));
        
        scanf("%[^\n]", input_string);
        
        // Clear the stdin buffer
        char ch;
        while((ch = getchar()) != '\n');

        //Check whether the input provided contains PS1 or not
        if(strstr(input_string, "PS1")){

            if(strchr(input_string, ' ') == NULL){
                strcpy(prompt, strstr(input_string, "PS1=") + 4);
            }
        }
        else{
            char *cmd = get_command(input_string);
            int cmd_type = check_command_type(cmd);

            if(cmd_type == BUILTIN){
                execute_internal_commands(input_string);
            }
            else if(cmd_type == EXTERNAL){
                execute_external_commands(input_string);
            }
            else{
                printf("%s: command not found\n", input_string);
            }
        }
    }
}