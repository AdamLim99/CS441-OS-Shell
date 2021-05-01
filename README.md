# CS441/541 Shell Project

## Author(s):

Shyue Shi Leong & Ze Jia Lim


## Date:

10/17/2020


## Description:

This is an implemention of a command line interpreter (CLI) or, as it is more commonly known, a shell. The shell should operate in this basic way: when user type in a command 
(in response to its prompt), the shell creates a child process that executes the command the user entered and then prompts for more user input when it has finished.


## How to build the software

1. Get into the folder/directory of cs441-shell.
2. Type "make" at the command line and then press enter to compile the code.


## How to use the software

1. Make sure you are in the cs441-shell's folder/directory to use the software.
2. There are 2 options available for this program, which are batch mode and interactive mode.
3. For BATCH mode, just you will need to type something like this: "./mysh tests/file1.txt"
   After the ./mysh, it MUST be a file. If the file is in other directory, you would
   need to provide the directory as well. There could be more than one file in the command line.
4. For INTERACTIVE mode, you will just need to type "./mysh" at the command line and then press enter.
   Interactive mode works just like how a normal shell would, but for this program, there are 5 built-in
   commands, which are jobs, history, wait, foreground (fg) and exit. You could run multiple commands with
   the help of the symbols ';' and/or '&'. ';' is if you want the command to be run at the foreground, and
   '&' is if you want the command to be run at the background. Eg: date ; sleep 5 & pwd
   a) jobs: In both modes of execution, it will display a list of the jobs that are currently running in the background.
   b) history: In both modes of execution, it will display the full history of job commands typed into the shell, including 
               the erroneous ones.
   c) wait: In both modes of execution, it will wait for ALL currently backgrounded jobs to complete. If there are no 
            backgrounded jobs, then the command returns immediately.
   d) fg: In both modes of execution, it will wait for a specific currently backgrounded job. fg takes zero or one arguments. 
          If there are zero arguments, then the latest backgrounded process is brought into the foreground and will not return  
          until the job has completed. If there is an argument, it will be the job id of the job to bring into the foreground, 
          as listed by the jobs command.
   e) exit: In both modes of execution,l it will terminates when it sees the exit command or reaches the end of the input stream, 
            or when the user types 'Ctrl-D'). If a background process is still running when the shell exits then the shell should 
            wait for that process to complete before exiting. In this case, your shell should print a message indicating the number  
            of jobs the shell is waiting on when given the exit command.


## How the software was tested

There are two categories of testing, one for batch mode and the other for interactive mode.
1. Batch Mode:
   a) First and foremost, the batch mode is tested with most of the test files given in the given-tests directory.
   b) Then, it is tested with the 5 test files that were written by ourselves.
   c) The content of the test files are very similar to the tests that was carried out for interactive mode, which 
      would be discussed below.
   d) fg is tested in test4.txt and file redirection is tested in test5.txt.
   e) Multiple files in a single command line was also tested.
2. Interactive Mode:
   a) The interactive mode is first tested with single commands first, such as date, pwd and ls. The built-in commands 
      were also tested while we were doing this test.
   b) Then, multiple commands that would be executed in the foreground was tested, eg: date; pwd ; ls. The built-in 
      commands were also tested while we were doing this test.
   c) Next, multiple commands that would be executed in the background was tested. eg: sleep 3 & sleep 5 & sleep 8.
   d) Lastly, multiple commands that would be executed in the foreground AND background was tested, eg: date ; sleep 5 & 
      pwd.   
   e) The software was also tested to handle some corner cases, which are empty command line, command line with only white spaces,
      white spaces before and/or after a command, invalid commands and Ctrl-D for exit.
   f) fg and file redirection are tested too, just like how it is tested in test5.txt.


## Test Suite

1. test1.txt - Simple test file to test basic and builtin commands, and to test if the program handles an empty string/new line.
2. test2.txt - Test for multiple commands with the ';' simple, and also test for background jobs with the '&' simple. Test if
	       the builtin exit command wait for all background jobs to be done before exiting.
3. test3.txt - A more robust test file that includes what test1.txt and test2.txt is testing. Also test for invalid command and
	       multiple (2) empty strings/new lines. Test if the builtin wait command is working as it should be.
4. test4.txt - Specifically test for fg. Also includes what test1.txt and test2.txt are testing.
5. test5.txt - Specifically test for file redirection. Also test for fg and what test1.txt and test2.txt are testing.


## Known bugs and problem areas

1. given-tests/level10.txt able to run but corrupted during execution
