#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARGS 64

void sigint_handler(int signo) {

	char cwd[150];
	printf("Caught SIGINT\n");
	//system("killall -9 -o 5s a.out");
	//system("exit");
	//kill(getpid(), SIGKILL);
	//system("./a.out");
	//signal(SIGINT, sigint_handler);
	getcwd(cwd, sizeof(cwd));
	char *username = getenv("USER");
//	printf("[" "\033[0;31m" "%s" "\033[0m" "]:" "\033[0;34m" "%s" "\033[0m" "\n" "\033[0m",  username, cwd);
}

void sigtstp_handler(int signo1) 
{
	printf("C1aught SIGTSTP\n");
	//system("killall -9 -o 5s a.out");
	//system("killall -9 a.out");
	//system("./a.out");
	//signal()
	
	signal(SIGTSTP,sigint_handler);
	kill(getpid(), SIGTSTP);
}

//Declaring functions
int cmd_type_check(char *args[]);


//Starting main
int main()
{
	//Prompt the welcome notice
	printf("--------------------------------------------\n");
	printf("Welcome to MINI-shell version\nAuthor : Sandeep\n");
	printf("--------------------------------------------\n");


	//Declare variables
	char array[50]; 
	int k, ret, i, cmd_type, ret1;
	char *args[MAX_ARGS], *temp = NULL;
	char **next = args;
	char cwd[150];

	//Get the username
	char *username = getenv("USER");


	//Update input array with NULL characters
	memset(array, '\0', 50);

	signal(SIGINT, sigint_handler);
	signal(SIGTSTP,sigtstp_handler);
//	while(1)
	{
	//	int ret1 = fork();
	//	if(ret1 == 0)
		{
			//Start while loop for running the mini shell
			while(1)
			{
				//Get the current working directory
				getcwd(cwd, sizeof(cwd));

				//Printing username + cwd in terminal with colours
				printf("[" "\033[0;31m" "%s" "\033[0m" "]:" "\033[0;34m" "%s\n" "\033[0m", username, cwd);
				

				printf("\033[0m"); 
				//Get the input
				gets(array);
				char arr[50];
				strncpy(arr,array,sizeof(array));
				printf("\033[0m"); 
				if(array[0] == '\0')
				{
					continue;
				}
				//Compare to exit
				if(strcmp(array, "exit") == 0)
				{
					exit(0);
				}

				//Get the tokens of input command with " "(space) as delimiter
				i = 0;
				args[i] = strtok(array, " ");
				while(args[i] != NULL)
				{
					i++;
					args[i] = strtok(NULL, " ");
				}
				args[i]  = NULL;


				//As "cd" is bullt-in command execvp cant execute. Execute it seperately
				if(strcmp(args[0], "cd") == 0)
				{
					chdir(args[1]);
					continue;
				}


				//Find which command is passed
				cmd_type = cmd_type_check(args);
				char buff[100];
				int fd;
				switch(cmd_type)
				{
					//1 for commands executed using execvp
					case 1:
						//Create a child process to execute system commands
						ret = fork();
						if(ret == 0)
						{
							//signal(SIGINT, sigint_handler);
							k = execvp(args[0], args);
							if(k == -1)
							{
								perror("execvp");
							}
						}
						else
						{
							while(wait(NULL) != -1);
						}
						memset(array, '\0', 50);
						break;
						//2 for pipes
					case 2:

						fd = open("c.txt", O_CREAT | O_RDWR, 00777);
						puts(arr);
						//array = "shubam k s";
						write(fd,arr,sizeof(arr));
						//system(args[0]args[1]args[2]);
						system("sh c.txt");
						printf("Pipes is found\n");
				}
			}
		}
	/*	else
		{
			wait(NULL);
		}*/
	}
}

//Defining cmd_type_check function
int cmd_type_check(char *args[])
{
	int i, pipe_count = 0;
	//Check for pipe
	for(i = 0; *(args + i) != NULL; i++)
	{
		if(strcmp(*(args + i), "|") == 0)
		{
			pipe_count++;
		}
	}
	if(pipe_count == 0)
	{
		return 1;
	}
	else
	{
		printf("No of pipes detected : %d\n", pipe_count);
		return 2;
	}
}



