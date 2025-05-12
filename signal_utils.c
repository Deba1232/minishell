#include "msh.h"

StoppedProcessList *head = NULL;
StoppedProcessList *tail = NULL;

void signal_handler(int signum){
    if(signum == SIGINT){   // Ctrl + C
        if(!child_pid){

            if(strncmp(prompt, "minishell", 9) == 0){
                printf(ANSI_COLOR_GREEN "\n%s@%s" ANSI_COLOR_RESET ":" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "$ ", prompt, host_name_buf,getcwd(pwd_buf, sizeof(pwd_buf)));
            }
            else{
                printf("%s", prompt);
            }
    
            // Flush output to get the prompt message immediately
            fflush(stdout);
        }
        else{
            printf("\n");
        }
    }
    else if(signum == SIGTSTP){   // Ctrl + Z
        if(!child_pid){

            if(strncmp(prompt, "minishell", 9) == 0){
                printf(ANSI_COLOR_GREEN "\n%s@%s" ANSI_COLOR_RESET ":" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "$ ", prompt, host_name_buf,getcwd(pwd_buf, sizeof(pwd_buf)));
            }
            else{
                printf("%s", prompt);
            }
    
            // Flush output to get the prompt message immediately
            fflush(stdout);
        }
        else{
            printf("\n");
            add_stopped_process(&head, child_pid, current_running_command, &tail);
        }
    }
}