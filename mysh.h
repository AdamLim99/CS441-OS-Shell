/*
 * Shyue Shi Leong and Ze Jia Lim
 *
 * CS441/541: Project 3
 *
 */
#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* For fork, exec, sleep */
#include <sys/types.h>
#include <unistd.h>
/* For waitpid */
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

/******************************
 * Defines
 ******************************/
#define TRUE  1
#define FALSE 0
#define MAX_COMMAND_LINE 1024
#define PROMPT ("mysh$ ")

/******************************
 * Structures
 ******************************/
/*
 * A job struct.  Feel free to change as needed.
 */
struct job_t {
    int argc;
    char ** argv;
    int is_background;
    char * binary;
    int is_redirection;
    int is_outfile;
    char * infile;
    char * outfile;
};
typedef struct job_t job_t;

/*
 * A back job struct
 */
struct backjob_t {
    pid_t processID;
    int index;
    int is_running;
    int argc;
    char ** argv;
    int displayed;
};
typedef struct backjob_t backjob_t;

/******************************
 * Global Variables
 ******************************/
 
/*
 * Interactive or batch mode
 */
int is_batch = FALSE;

/*
 * Counts
 */
int total_jobs_display_ctr = 0;
int total_jobs    = 0;
int total_jobs_bg = 0;
int total_history = 0;

/*
 * Debugging mode
 */
int is_debug = TRUE;

int max_jobs=100;
int numArg;
char ** allArg;
int start=0;
int finish=0;
int startNum2=0;
job_t * jobArray;
backjob_t * backArray;

/******************************
 * Function declarations
 ******************************/
/*
 * Parse command line arguments passed to myshell upon startup.
 *
 * Parameters:
 *  argc : Number of command line arguments
 *  argv : Array of pointers to strings
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int parse_args_main(int argc, char **argv);

/*
 * Main routine for batch mode
 *
 * Parameters:
 *  None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int batch_mode(void);

/*
 * Main routine for interactive mode
 *
 * Parameters:
 *  None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int interactive_mode(void);

/*
 * Handle the line read in either from stdin or from a file
 *
 * Paramaters:
 *  A char array containing the line read
 *
 * Returns:
 *  0 to continue the program
 *  1 to exit the program
 *  Negative value on error
 */
int executeLine(char * line);

/* 
 * Handle the unnecessary white spaces
 * 
 * Parameters:
 *  A char array containing the line that we want remove white spaces
 * 
 * Returns:
 *  A char array containing the line without unnecessary white spaces
 */
char * removeSpace(char * line, char * results);

/* 
 * Handle the argument in the job array before execvp()
 * 
 * Parameters:
 *  A char array containing the argument for execvp()
 *
 * Returns:
 *  None
 */
void handleArgs(char * line, int finish);

/*
 * Launch a job
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error 
 */
int launch_job();

/*
 * Built-in 'exit' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_exit(void);

/*
 * Built-in 'jobs' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_jobs(void);

/*
 * Built-in 'history' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_history(void);

/*
 * Built-in 'wait' command
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_wait(void);

/*
 * Built-in 'fg' command
 *
 * Parameters:
 *   None (use default behavior)
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_fg(void);

/*
 * Built-in 'fg' command
 *
 * Parameters:
 *   Job id
 *
 * Returns:
 *   0 on success
 *   Negative value on error
 */
int builtin_fg_num(int job_num);

#endif /* MYSHELL_H */
