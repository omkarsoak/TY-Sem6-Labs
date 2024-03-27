/*************************
OMKAR OAK 112103099
OS LAB ASSIGNMENT 2: SHELL
**************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h> 
#include <signal.h>

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITERS1 " \t\n"    //used for options and arguments
#define TOKEN_DELIMITERS2 ":="      //used for paths
#define USER_PROMPT 0
#define NORMAL_PROMPT 1

//function prototypes
char** tokenize_string(char*,char*);
char** getargs();
int builtin_cd(char**);
int executeShell(char**,char**);


char** tokenize_string(char* line,char* token_delimiters)
{
	int bufsize = TOKEN_BUFSIZE;
	int position = 0;
	char** arguments = malloc(bufsize * sizeof(char*));
	char* token;

	if (!arguments) 
	{
		perror("Shell says");
		exit(errno);
	}

	token = strtok(line, token_delimiters);   //divide tokens seperated by delimiters
	
	while (token != NULL) 
	{
		arguments[position] = token;
		position++;

		if (position >= bufsize) 
		{
			bufsize += TOKEN_BUFSIZE;
			arguments = realloc(arguments, bufsize*sizeof(char*));
			if (!arguments) 
			{
				perror("Shell says");
				exit(errno);
			}
		}

		token = strtok(NULL, token_delimiters);
	}
	arguments[position] = NULL;  //null terminate the array
	return arguments;
}

//get arguments from the line
char** getargs()
{
	//gets line from stdin
	char* line = NULL;
	long line_bufsize = 0;

	if (getline(&line, &line_bufsize, stdin) == -1)
	{
		if (feof(stdin)) 
		{
			exit(0);  // 0 = success, EOF happened
		} 
		else  
		{
			perror("Shell says");
			exit(errno);
		}
	}

	char** arguments = tokenize_string(line,TOKEN_DELIMITERS1);  //seperates options and arguments

	return arguments;
}

int builtin_cd(char** args)
{
	if (args[1] == NULL) 
	{
		perror("Shell says");
		exit(errno);
	} 
	else 
	{
		if (chdir(args[1]) != 0)  //change directory to args[1]
		{
			perror("Shell says");
			//exit(errno);
		}
	}
	return 1;
}

int executeShell(char** args, char** PATHS) {
    char complete_path[256];
    if (args[0] == NULL) {
        return 1; // No command was entered
    }

    // Check for built-in functions: cd, exit
    if (strcmp(args[0], "cd") == 0) {
        return builtin_cd(args);
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
        return 0;
    }

    int pid;
    int status = -1;

    pid = fork(); // fork - system call
    if (pid == 0) {
        // Iterate over args to check for input and output redirection
        for (int j = 1; args[j] != NULL; j++) {
            if (strcmp(args[j], "<") == 0) { // Input redirection
                int fd = open(args[j + 1], O_RDONLY);
                if (fd == -1) {
                    perror("Shell says");
                    exit(errno);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[j] = NULL; // Remove "<"
                args[j + 1] = NULL;
            } else if (strcmp(args[j], ">") == 0) { // Output redirection
                int fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("Shell says");
                    exit(errno);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[j] = NULL; // Remove ">"
                args[j + 1] = NULL;
            } else if (strcmp(args[j], "|") == 0) { // Pipe
                // Create a pipe
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    perror("Pipe failed");
                    exit(errno);
                }

                pid_t child_pid = fork();
                if (child_pid == -1) {
                    perror("Fork failed");
                    exit(errno);
                } else if (child_pid == 0) {
                    // Child process: redirect output to pipe
                    close(pipefd[0]); // Close read end of the pipe
                    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
                    close(pipefd[1]); // Close write end of the pipe

                    // Execute command before the pipe symbol
                    char** left_args = args;
                    left_args[j] = NULL; // Terminate args before the pipe symbol
                    executeShell(left_args, PATHS);
                    exit(0);
                } else {
                    // Parent process: redirect input from pipe
                    close(pipefd[1]); // Close write end of the pipe
                    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
                    close(pipefd[0]); // Close read end of the pipe

                    // Execute command after the pipe symbol
                    char** right_args = args + j + 1;
                    executeShell(right_args, PATHS);
                    exit(0);
                }
            }
        }

        // If no pipe symbol found, execute single command
        for (int i = 0; PATHS[i] != NULL; i++) {
            snprintf(complete_path, sizeof(complete_path), "%s/%s", PATHS[i], args[0]);
            if (access(complete_path, X_OK) == 0) {
                execv(complete_path, args); // Execute command
                perror("execv");
                exit(errno);
            }
        }
        fprintf(stderr, "Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        waitpid(pid, &status, 0);
    }

    return 1;
}



int main() 
{
	int flag = 1;
	int prompt_flag = NORMAL_PROMPT;

	//getting paths from echo $PATH
	char** PATHS = malloc(64 * sizeof(char*));
	int path_counter = 0;
	char echo_paths[256] = "/home/omkar/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin";
	PATHS = tokenize_string(echo_paths,":");
	while (PATHS[path_counter]!=NULL){path_counter++;}

	while(flag == 1) 
	{
		char *userName = getlogin();
		char hostname[128];
		gethostname(hostname, sizeof(hostname));
		char cwd[256];
		getcwd(cwd, sizeof(cwd));

		char userPrompt[512];

		//printf("%s@%s:~%s$ ",userName,hostname,cwd);
		if(prompt_flag == NORMAL_PROMPT)  //print normal prompt
		{
			snprintf(userPrompt, 512, "%s@%s:~%s$ ", userName,hostname,cwd);  //concatenate
		}
		printf("%s",userPrompt);

		char** args = getargs();    //IMP: tokenize and get the arguments
		
		if(args[0]==NULL)   //only pressed enter
		{
			continue;
		}
		

		/*allows the user to change the prompt to a particular string and revert back to normal prompt*/
		if(strcmp(args[0],"PS1=\\w$")==0)   //revert back to normal prompt
		{
			prompt_flag = NORMAL_PROMPT;
		}
		else if(strncmp(args[0], "PS1=",4) == 0)    //change prompt to some string, eg. "PS1=omkar" = omkar> 
		{
			strcpy(userPrompt, args[0] + 4);
			strcat(userPrompt,"> ");
			prompt_flag = USER_PROMPT;
		}
		
		//similar to 'export PATH=$PATH:/home/omkar/Desktop/omkar/OS_Lab:/home/omkar'
		else if (strncmp(args[0],"PATH=",4) == 0)
		{
			char** arguments = tokenize_string(args[0],TOKEN_DELIMITERS2);

			int i=1;
			while(arguments[i]!=NULL)
			{
				//printf("%s\n",arguments[i]);
				PATHS[path_counter] = arguments[i];
				i++;path_counter++;
			}
			PATHS[path_counter] = NULL;
		}
		else
		{
			flag = executeShell(args,PATHS);    //execute the shell (contains fork-exec)
		}
		
		free(args);
	}
	return 0;
}



