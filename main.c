#include "msh.h"

int main(){
    system("clear");
    
    //Prompt to display
    char prompt[20] = "minishell";

    char input_string[20];

    scan_input(prompt, input_string);

    for(int i = 0; i < external_cmd_count; i++){
        free(external_commands[i]);
    }

    return 0;
}