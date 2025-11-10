#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int reply =	system(cmd);
    
     if (reply == 0) {
        printf("Command executed successfully.");
    }
    else {
        printf("Command execution failed or returned "
               "non-zero: %d", reply);
	       
	       return false;
    }   
    

    return true;
}


/*
    int returnCode = system("echo Hello, World!");

  // checking if the command was executed successfully
    if (returnCode == 0) {
        printf("Command executed successfully.");
    }
    else {
        printf("Command execution failed or returned "
               "non-zero: %d", returnCode);
    }

    return 0;*/

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/


bool do_exec(int count, ...)
{
    pid_t pid = fork();


        va_list args;
     va_start(args, count);
    
    char * command[count+1];

    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
    
    char * path = command[0];

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return false;
    }

    else if (pid == 0) {
        // Child process
        //printf("pid is 0\n");

	//int result = execv(path, commands_II);
	//int result = execv(path, command);
	if (execv(path, command)== -1){
	//if (execv(path, commands_II)== -1){
 
           perror("execv error");
            exit(EXIT_FAILURE);
        }

 
        }
     else {
        // Parent process
        int status;
        //pid_t wpid = wait(&status);
	pid = waitpid(pid,&status,0); 

        if (pid == -1) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }

        // Check how the child terminated
        if (WIFEXITED(status)) {
	    if (WEXITSTATUS(status)==1){
            printf("Command %s returned non zero value %d\n", path, WEXITSTATUS(status));
	     return false;
	     }
        } else if (WIFSIGNALED(status)) {
            printf("Parent: child terminated by signal %d\n", WTERMSIG(status));
        }
    }

    va_end(args);
    return true;    
    }    
    
/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    pid_t pid = fork();

    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


    char * path = command[0];

 /*   int filedes = open(outputfile, O_WRONLY);

    if (filedes <0){
    	perror("file open");
	return 1;
	}
	
    //redirection configuration
    if (dup2(filedes, STDOUT_FILENO) <0){
    	perror("dup2");
	return 1;
	}*/

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return false;
    }

    else if (pid == 0) {
        // Child process
        printf("pid is 0\n");


   int filedes = open(outputfile, O_WRONLY);

    if (filedes <0){
    	perror("file open");
	return 1;
	}
	
    //redirection configuration
    if (dup2(filedes, STDOUT_FILENO) <0){
    	perror("dup2");
	return 1;
	}

	if (execv(path, command)== -1){

           perror("execv error");
            exit(EXIT_FAILURE);
        }
	close(filedes);
 
        }
     else {
        // Parent process
        int status;
	pid = waitpid(pid,&status,0); 

        if (pid == -1) {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }

        // Check how the child terminated
        if (WIFEXITED(status)) {
            printf("Parent: child exited with status %d\n", WEXITSTATUS(status));
	    if (WEXITSTATUS(status)==1) return false;
        } else if (WIFSIGNALED(status)) {
            printf("Parent: child terminated by signal %d\n", WTERMSIG(status));
        }
    }

    va_end(args);
    return true;    
    }    



/*
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];
*/

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

/*    va_end(args);

    return true;
}
*/



