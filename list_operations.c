#include "msh.h"

int stopped_process_count;

void add_stopped_process(StoppedProcessList **head, int child_pid, char *command, StoppedProcessList **tail){
    StoppedProcessList *stopped_process_node = (StoppedProcessList *)calloc(1, sizeof(StoppedProcessList));
    if(!stopped_process_node){
        perror("Memory couldn't be allocated");
        exit(EXIT_FAILURE);
    }

    stopped_process_node->prev = NULL;
    stopped_process_node->child_process_id = child_pid;
    stopped_process_node->stopped_command = strdup(command);    // Duplicate the command string to prevent issues if the original command gets overwritten later                                         
    stopped_process_node->next = NULL;

    if(!*head && !*tail){
        *head = *tail = stopped_process_node;
    }
    else{
        (*tail)->next = stopped_process_node;
        stopped_process_node->prev = *tail;
        *tail = stopped_process_node;
    }

    stopped_process_count++;
}

void remove_stopped_process(StoppedProcessList **head, StoppedProcessList **tail){
    StoppedProcessList *temp = *tail;

    if(!temp->prev && !temp->next){
        *head = NULL;
        *tail = NULL;
        free(temp);
    }
    else{
        *tail = temp->prev;
        (*tail)->next = NULL;

        temp->prev = NULL;
        temp->next = NULL;
        free(temp);
    }

    stopped_process_count--;
}

void print_stopped_processes(StoppedProcessList *head, StoppedProcessList *tail){
    int count = 1;

    StoppedProcessList *temp = head;
    while(temp){
        printf("[%d]   Stopped                 %s\n", count, temp->stopped_command);
        count++;
        temp = temp->next;
    } 
}