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
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern char *external_commands[200]; /* Stores the external commands*/
extern int external_cmd_count; /* Amount of external commands*/

extern int status; /* Child exit status code*/

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
 * @brief Executes external commands provided
 */
void execute_external_commands(char *);
void signal_handler(int sig_num);

/**
 * @brief Extracts the external commands from a list and store it in an array
 */
void extract_external_commands(char **);

#endif