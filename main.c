#include "msh.h"

char prompt[20];
char input_string[20];

int main(){
    system("clear");
    
    //Prompt to display
    strcpy(prompt, "minishell");

    scan_input(prompt, input_string);

    for(int i = 0; i < external_cmd_count; i++){
        free(external_commands[i]);
    }

    return 0;
}
