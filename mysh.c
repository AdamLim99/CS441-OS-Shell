/*
 * Author: Shyue Shi Leong & Ze Jia Lim
 * CS441/541: Project 3 - Shell
 */
#include "mysh.h"

/* 
 * The main method of the program. It takes in two arguments and imitate the
 * execution of a shell.
 *
 * Parameters:
 *  an integer storing the number of arguments in the command line
 *  an array of strings containing the command line arguments
 *
 * Returns :
 *  0 if success
 * -1 if error occurred
 */
int main(int argc, char * argv[]) {
    int ret;
    int i,j,k;

    /*
     * Parse Command line arguments to check if this is an interactive or batch
     * mode run.
     */
    if( 0 != (ret = parse_args_main(argc, argv)) ) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }

    /*
     * If in batch mode then process all batch files
     */
    if( TRUE == is_batch) {
        if( TRUE == is_debug ) {
            printf("Batch Mode!\n");
        }

        if( 0 != (ret = batch_mode()) ) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    }
    /*
     * Otherwise proceed in interactive mode
     */
    else if( FALSE == is_batch ) {
        if( TRUE == is_debug ) {
            printf("Interactive Mode!\n");
        }

        if( 0 != (ret = interactive_mode()) ) {
            fprintf(stderr, "Error: Interactive mode returned a failure!\n");
        }
    }
    /*
     * This should never happen, but otherwise unknown mode
     */
    else {
        fprintf(stderr, "Error: Unknown execution mode!\n");
        return -1;
    }


    /*
     * Display counts
     */
    printf("-------------------------------\n");
    printf("Total number of jobs               = %d\n", total_jobs);
    printf("Total number of jobs in history    = %d\n", total_history);
    printf("Total number of jobs in background = %d\n", total_jobs_bg);

    /*
     * Cleanup
     */
    for(i=0;i<total_history;i++){
        for(k=0;k<jobArray[i].argc;k++){
            free(jobArray[i].argv[k]);
        }
        free(jobArray[i].argv);
        free(jobArray[i].binary);
    }
    for(j=0;j<total_jobs_bg;j++){
        for(k=0;k<backArray[i].argc;k++){
            free(backArray[i].argv[k]);
        }
        free(backArray[i].argv);
    }
    free(jobArray);
    free(backArray);

    return 0;
}

/*
 * This method mainly determines whether the process wants to be done in either batch mode or 
 * interactive mode.
 *
 * Parameters: 
 *  an integer storing the number of command line arguments
 *  an array of strings containing the command line arguments
 *
 * Returns:
 *  0 if success
 */
int parse_args_main(int argc, char **argv)
{
    //declaring local variables
    int i;
    numArg = argc;

    //handle and storing the command line arguments
    if(argc>1){
        allArg = (char **)malloc(sizeof(char *)*argc-1);
        for(i=0;i<argc-1;i++){
            allArg[i] = argv[i+1];
        }
    }

    /*
     * If no command line arguments were passed then this is an interactive
     * mode run.
     */
    if(argc == 1){
        is_batch = FALSE;
    }
    /*
     * If command line arguments were supplied then this is batch mode.
     */
    else{
        is_batch = TRUE;
    }
    return 0;
}

int batch_mode(void)
{
    /*
     * For each file...
     */
        /*
         * Open the batch file
         * If there was an error then print a message and move on to the next file.
         * Otherwise, 
         *   - Read one line at a time.
         *   - strip off new line
         *   - parse and execute
         * Close the file
         */

    // declare local variables
    FILE* file=NULL;
    char* line = (char *)malloc(sizeof(char)*MAX_COMMAND_LINE);
    int i;
    int endCon=0;

    // allocating space for data structures
    jobArray=(job_t *)malloc(sizeof(job_t)*max_jobs);
    backArray=(backjob_t *)malloc(sizeof(backjob_t)*max_jobs);

    // open a file, read each line, executing each line and close it
    // repeat until all file is read and skip any file that are unable to open
    for(i=0;i<numArg-1;i++){
        file=fopen(allArg[i], "r");
        if(file==NULL){
            fprintf(stderr,"Unable to open %s file\n",allArg[i]);
        }
        else{
	    do{
		fgets(line, MAX_COMMAND_LINE, file);
		if(feof(file)){
		    break;
		}
		if(strcmp(line,"\n")!=0){
		    line[strlen(line)-1]='\0';
		    line=removeSpace(line, line);
		    if(strcmp(line, "")!=0){
		        endCon=executeLine(line);
		    }
                }
	    }while(endCon==0);
            fclose(file);
        }
    }
    free(line);
    return 0;
}

int interactive_mode(void)
{
    //do {
        /*
         * Print the prompt
         * Read stdin, break out of loop if Ctrl-D
	 * Strip off the newline
	 * Parse and execute the command
         */
    //} while( 1/* end condition */);
    // declare local variables
    size_t bufferSize = MAX_COMMAND_LINE;
    char * buffer = (char *)malloc(sizeof(char) * bufferSize);
    int endCondition=0;

    // allocating space for data structures
    jobArray = (job_t *)malloc(sizeof(job_t) * max_jobs);
    backArray = (backjob_t *)malloc(sizeof(backjob_t) * max_jobs);

    // constantly printing out the prompt, read the user input and executing it
    // user can exit via exit command or Ctrl + D
    do{
        printf("%s", PROMPT);
        getline(&buffer,&bufferSize,stdin);
	buffer[strlen(buffer)-1]='\0';
        buffer = removeSpace(buffer, buffer);

        if(feof(stdin)){
            printf("\n");
	    exit(0);
        }
        
        if(strcmp(buffer,"") != 0){
            endCondition = executeLine(buffer);
        }
        
    }while(endCondition==0);

    /*
     * Cleanup
     */
    free(buffer);
    return 0;
}

/*
 * You will want one or more helper functions for parsing the commands 
 * and then call the following functions to execute them
 */
int executeLine(char * command){
    // declare local variables
    char * token;
    char * token2;
    int temp=0;
    int i=start;
    int results=0;
    char * line = strdup(command);

    // check if the line contains ;. If yes, then it will enter the else case
    // Enter if case if no ; is found in line
    if(strstr(line,";")==NULL){
        // check for & in the line. If yes, enters the if case. Enter else case otherwise
        if(strstr(line, "&") != NULL){
            // line contains only &
            // check the last job whether it is foreground or background
            temp = strlen(line);
            char lastchr[] = {line[temp-1],'\0'};
            
            if(strcmp(lastchr,"&")!=0){
                temp = -1;
            }
            
            // remove the unnecessary white spaces and handles the input for the launch_job method
            // increment global counters
            while((token2 = strtok_r(line, "&", &line))){
                token2 = removeSpace(strdup(token2),strdup(token2));
                handleArgs(strdup(token2), finish);
                jobArray[finish].is_background = 1;
                finish++;
                total_jobs_bg++;
		total_history++;
             }
            
            if(temp == -1){
                //set the last job to be executed in the forefront cause no & detected at the end
                jobArray[total_history-1].is_background = 0;
                total_jobs_bg--;
            }

        }
        else{ 
            // line contains neither & or ;
            // handle the input for launch_job method and increment global counters
	    line = removeSpace(line,line);
            handleArgs(line,finish);
            jobArray[finish].is_background = 0;
            finish++;
            total_history++;
        }

    }
    else{
        // line contains a mixture of & and ;
        while((token = strtok_r(line, ";", &line))){
            // check if & is present in the line. If yes, then enter if case. Enter else case otherwise
            if(strstr(token, "&") != NULL){
                temp = strlen(token);
                char lastchr[] = {token[temp-1],'\0'};
                if(strcmp(lastchr,"&")!=0){
                    temp = -1;
                }
                                 
                while((token2 = strtok_r(token,"&",&token))){
                    // handle argument for launch_jobs method and increment global counters
                    token2 = removeSpace(strdup(token2),strdup(token2));
                    handleArgs(strdup(token2), finish);
                    jobArray[finish].is_background = 1;
                    finish++;
                    total_jobs_bg++;
                    total_history++;
                }
                if(temp == -1){
                    //set the last job to be executed in the forefront cause no & detected at the end
                    jobArray[total_history-1].is_background = 0;
                    total_jobs_bg--;
                }
            }
            else{
                // there is no & meaning no background process for this line
                // handle arguments and increment global counters
		if(token != NULL){
		    if(is_batch == FALSE){
                        token = removeSpace(token,token);
		    }
                    handleArgs(token,finish);
                    jobArray[finish].is_background = 0;
		    finish++;
		    total_history++;
	        }
            }
        }
    }
    
    // launch each individual job that we have created in this method call
    for(i=start;i<total_history;i++){       
        // launch the job
        results = launch_job();
        
        // check for error or exit command
        if(results == -1){
            fprintf(stderr,"An error occurred when executing job number %d\n", start);
        }
        if(results == 1){
            break;
        }
        // increment the starting index 
        start++;
    }
    // reallocating more space if total number of jobs exceed a certain amount
    if((total_history%100) == 80){
	max_jobs = max_jobs * 2;
	jobArray = (job_t *)realloc(jobArray, sizeof(job_t) *max_jobs);
    }	

    // set the global starting index and prepare for the next time this method is called
    start = total_history;
    return results;
}

char * removeSpace(char * line, char * results){
    int i;
    int j;
    int k;
    int index=0;
    int length = strlen(line);
    
    // use loop to find the index of the first non null character starting from the end of the string
    for(i=length-1;i>=0;i--){
        if(line[i] != ' '){
            break;
        }
    }
    
    // return empty string if the line is empty
    if(i==0){
        return "";
    }

    // use loop to find the index of thefirst non null charatcer starting from the front of the string
    for(j=0;j<i+1;j++){
        if(line[j] != ' '){
	    break;
        }
    }
    
    // copy the string from the starting index to the end index
    for(k=j;k<i+1;k++){
	results[index] = line[k];
	index++;
    }

    results[index] = '\0';
    return results;
}

void handleArgs(char * line, int finish){
    char * token;
    char * check = " ";
    char * ret;
    int count=0;
    int i=0;
    char * check2 = "<";
    char * check3 = ">";

    // check if there is space in the line to determine how many arguments in the line
    line = removeSpace(line,line);
    ret = strstr(line, check);
    
    if(ret != NULL){
	for(i=0;i<strlen(line);i++){
	    if(isspace(line[i])!=0){
                count++;
	    }
        }
	
	count = count + 2; 
        token = strtok(line, check);
        jobArray[finish].argc = count-1;
        jobArray[finish].argv = (char **)malloc(sizeof(char *) * count);
        
 	for(i=0;i<count-1;i++){
	    if(token == NULL){
		break;
	    }
            jobArray[finish].argv[i] = strdup(token);
            token = strtok(NULL, check);
        }
        jobArray[finish].is_redirection=0;
        jobArray[finish].is_outfile=0;
        jobArray[finish].argv[count-1] = NULL;
        
	if(strcmp(jobArray[finish].argv[1], check2)==0 && count>3){
            jobArray[finish].is_redirection=1;
            jobArray[finish].infile = strdup(jobArray[finish].argv[2]);
        }
      	else if(strcmp(jobArray[finish].argv[1], check3)==0 && count>3){
	    jobArray[finish].is_redirection=1;
            jobArray[finish].is_outfile=1;
            jobArray[finish].outfile = strdup(jobArray[finish].argv[2]);
        }
    }
    else{
        jobArray[finish].argc = 1;
        jobArray[finish].argv = (char **)malloc(sizeof(char *) * 2);
        jobArray[finish].argv[0] = strdup(line);
        jobArray[finish].argv[1] = NULL;
    }
    jobArray[finish].binary = jobArray[finish].argv[0];
}

int launch_job(){
    /*
     * Display the job
     * Launch the job in either the foreground or background
     * Some accounting
     */

    pid_t c_pid = 0;
    int status = 0;
    int i;
    int jobNum;
    char *ptr;
    int newfd;
    
    if(strcmp(strdup(jobArray[start].binary), "exit")==0){
        return builtin_exit();
    }
    else if(strcmp(jobArray[start].binary, "jobs")==0){
        return builtin_jobs();
    }
    else if(strcmp(jobArray[start].binary, "history")==0){
        return builtin_history();
    }
    else if(strcmp(jobArray[start].binary, "wait")==0){
        return builtin_wait();
    }
    else if(strcmp(jobArray[start].binary, "fg")==0){
       	if(jobArray[start].argc == 1){
            return builtin_fg();
        }
        else{
            jobNum = strtol(jobArray[start].argv[1],&ptr,10);
            return builtin_fg_num(jobNum);
        }
    }
    else{
        total_jobs++;
        
        // create the process
        c_pid = fork();

        // display error message when an error occurs
        if(c_pid < 0){
            fprintf(stderr,"Error: Fork failed!\n");
        }
        // child process
        else if(c_pid == 0){
            // handling redirection
	    if(jobArray[start].is_redirection == 1){
                if(jobArray[start].is_outfile == 1){
                    if((newfd = open(jobArray[start].outfile, O_RDWR))< 0){
                        fprintf(stderr,"Could not open outfile descripter!\n");
                        exit(-1);
                    }
		    dup2(newfd, 1);
		    close(newfd);
                }
                else{
                    if((newfd = open(jobArray[start].infile, O_RDONLY)) < 0){
                        fprintf(stderr,"Could not open infile descriptor!\n");
                        exit(-1);
                    }
		    dup2(newfd, 0);
		    close(newfd);
                }
            }

            // execute the process
            execvp(jobArray[start].binary,jobArray[start].argv);
            // print error message when execvp fails and kill the child process
            fprintf(stderr,"A command does not exist or cannot be executed\n");
            exit(-1);
        }
        // parent process
        else{
            // wait for the child to complete if the process is foreground
            if(jobArray[start].is_background == 0){
                waitpid(c_pid, &status, 0);
            }
        }
        // keeping track of the background job
        if(jobArray[start].is_background == 1){
	    backArray[startNum2].processID = c_pid;
	    backArray[startNum2].index = startNum2 + 1;
            backArray[startNum2].is_running = 1;
            backArray[startNum2].argc = jobArray[start].argc;

            backArray[startNum2].argv = (char **)malloc(sizeof(char *) * (jobArray[start].argc+1));
            for(i=0;i<jobArray[start].argc;i++){
                backArray[startNum2].argv[i] = strdup(jobArray[start].argv[i]);
            }
            backArray[startNum2].argv[i+1] = NULL;
            backArray[startNum2].displayed = 0;
	    startNum2++;

	    // reallocating more space if total number of background jobs exceed a certain amount
    	    if((total_jobs_bg%100) == 80){
	    	max_jobs = max_jobs * 2;
	 	backArray = (backjob_t *)realloc(backArray, sizeof(backjob_t) *max_jobs);
    	    }
        }
    }
    
    return 0;
}

int builtin_exit(void)
{
    if(total_jobs_bg !=0){
        if(total_jobs_bg!=total_jobs_display_ctr){
            fprintf(stdout, "Waiting for %d jobs to complete before exiting.\n", (total_jobs_bg-total_jobs_display_ctr));
        }
        builtin_wait();
    }
    return 1;
}

int builtin_jobs(void)
{
    int i;
    int j;
    pid_t returnpid;

    for(i=0;i<total_jobs_bg;i++){
	returnpid = waitpid(backArray[i].processID, NULL, WNOHANG);
        if(returnpid == 0){
            printf("[%d]   Running\t", backArray[i].index);
	    for(j=0;j<backArray[i].argc;j++){
		printf("%s ",backArray[i].argv[j]);
	    }
	    printf("\n");
        }
        else{
            if(backArray[i].displayed == 0){
                printf("[%d]   Done\t", backArray[i].index);
                for(j=0;j<backArray[i].argc;j++){
		    printf("%s ",backArray[i].argv[j]);
	        }
	        printf("\n");

	        backArray[i].displayed = 1;
	 	backArray[i].is_running = 0;
                total_jobs_display_ctr++;
            }
        }
    }
    return 0;
}

int builtin_history(void)
{
    int i;
    int j;
    for(i=0;i<total_history;i++){
        printf(" %d\t", i+1);
	for(j=0;j<jobArray[i].argc;j++){
            if(jobArray[i].argv[j] == NULL){
		 break;
	    }
	    printf("%s ",jobArray[i].argv[j]);
        }
        if(jobArray[i].is_background==1){
            printf("&");
        }
        printf("\n");
    }
    return 0;
}

int builtin_wait(void)
{
    int i;
    for(i=0;i<total_jobs_bg;i++){
        waitpid(backArray[i].processID, NULL, 0);
	    backArray[i].is_running = 0;
    }
    return 0;
}

int builtin_fg(void)
{
    int i;
    pid_t returnpid;

    for(i=total_jobs_bg - 1; i>-1;i--){
        returnpid = waitpid(backArray[i].processID,NULL,WNOHANG);
        if(returnpid == 0){
            break;
        }
    }
    
    returnpid = waitpid(backArray[i].processID,NULL,0);
    backArray[i].displayed=1;
    total_jobs_display_ctr++;
    if(returnpid < 0){
        fprintf(stderr, "No background process to execute!\n");
    }
    else{
        backArray[i].is_running=0;
    }
    return 0;
}

int builtin_fg_num(int job_num)
{
    pid_t returnpid;
    if(job_num < 0 && job_num > total_jobs_bg){
        fprintf(stderr,"Invalid job number!\n");
        return 0;
    }
    returnpid = waitpid(backArray[job_num-1].processID,NULL,0);
    backArray[job_num-1].displayed=1;
    total_jobs_display_ctr++;
    if(returnpid < 0){
        fprintf(stderr,"This process has already completed or invalid job number!\n");
    }
    backArray[job_num-1].is_running=0;
    return 0;
}
