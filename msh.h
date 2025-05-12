#ifndef MSH_H
#define MSH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 
#define EMPTY_COMMAND   0
#define BUILTIN		    1
#define EXTERNAL	    2
#define NO_COMMAND      3

#define PIPE_READ_END   0
#define PIPE_WRITE_END  1

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* A double linked list to store all the stopped processes and their respective PIDs */
typedef struct StoppedProcessList{
    struct StoppedProcessList *prev;
    pid_t child_process_id;
    char *stopped_command;
    struct StoppedProcessList *next;
} StoppedProcessList;

extern char prompt[20]; /* Stores the prompt string */
extern char input_string[20]; /* Stores the user input string */
extern char pwd_buf[100];   /* Stores the present working directory string */
extern char host_name_buf[50];  /* Stores the host name string */

extern char *external_commands[200]; /* Stores the external commands */
extern char current_running_command[256];
extern int external_cmd_count; /* Amount of external commands */

extern int status; /* Child exit status code */
extern pid_t child_pid; /* PID of the child process created to execute the external commands */

extern StoppedProcessList *head;    /* Pointer to the first node in the list of stopped processes */
extern StoppedProcessList *tail;    /* Pointer to the last node in the list of stopped processes */
extern int stopped_process_count;   /* Count the stopped processes */

/**
 * @brief Scan the input provided by the user
 */
void scan_input(char *, char *);

/**
 * @brief Get the command given as input
 * @returns The command string 
 */
char *get_command(char *);

void copy_change(char *prompt, char *input_string);

/**
 * @brief Checks the command type
 * @returns Command type macro 
 */
int check_command_type(char *);
void echo(char *input_string, int status);

/**
 * @brief Executes internal commands provided
 */
void execute_internal_commands(char *);

/**
 * @brief Executes external commands containing pipes
 */
void execute_piped_external_commands(char **, int, int);

/**
 * @brief Executes external commands provided
 */
void execute_external_commands(char *);

/**
 * @brief Handle the SIGINT and SIGTSTP signals
 */
void signal_handler(int);

/**
 * @brief Extracts the external commands from a list and store it in an array
 */
void extract_external_commands(char **);

/**
 * @brief Inserts at last any stopped process
 */
void add_stopped_process(StoppedProcessList **, int, char *, StoppedProcessList **);

/**
 * @brief Deletes the last stopped process from the list
 */
void remove_stopped_process(StoppedProcessList **, StoppedProcessList **);

/**
 * @brief Prints all stopped processes
 */
void print_stopped_processes(StoppedProcessList *, StoppedProcessList *);

#endif