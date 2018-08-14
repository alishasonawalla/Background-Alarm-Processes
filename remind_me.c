/*
 * Author: Alisha Sonawalla
 * Date: April 29, 2018
 * 
 * Description: This program displays the message inputted by the user after the time specified 
 *              by the user, while allowing the user to retain control of the command line.
 *              
 * Note: The upper case functions - Fork, Signal and Pause are from csapp.c. Those functions take care of 
 *       error handling and therefore, I haven't done it in my program
 */
 
 
//Include header file with declarations and libraries
#include "csapp.h"

//Declare a global char pointer to store message input by user
char *message = NULL; 

//Handler function to catch the alarm signal
void handler(int sig)
{
	//Flush the standard output buffer so it doesn't wait for the cache to fill
	fflush(stdout);
	printf("%s\n", message);
}


int main(int argc, char ** argcv, char ** envp)
{
	//If the user inputs less arguments or a negative time value exit the program with message
	if(argc < 3 || atoi(argcv[1]) <= 0)
	{
		fprintf(stderr, "Invalid input! Expected: filename seconds(non-negative) message");
		exit(1);
	}

	//Take time input from user
	int time = atoi(argcv[1]);

	//Fields for calculating size of input message by user
	int size = 0, i = 0;

	//If the user has types quotes, remove the quotes before outputting message
	if(strlen(argcv[2]) > 0 && argcv[2][0] == '"')
	{
		//Allocate space for the first word of the message
		size = (strlen(argcv[2]));	
		message = (char *) malloc(size);
		
		//If the message has more than just ' " '
		if(size > 1)
		{
			//Copy everything after the quote
			strcpy(message, &argcv[2][1]);	
		}
	}
	
	//Otherwise malloc the size of the first word input by the user +1
	//We need to add +1 for the null terminator
	else
	{
		size = (strlen(argcv[2]) + 1);
		message = (char *) malloc(size);
		strcpy(message, argcv[2]);
	}

	//For all command line arguments after the first word of the message 
	//Inputted by the user, use realloc to allocate space
	for(i = 3; i < argc; ++i)
	{
		size += (strlen(argcv[i]) + 2);
		message = (char *) realloc(message, size);
		//Add space after each word of the message
		strcat(message, " ");
		strcat(message, argcv[i]);
	}

    //Check if the last character also contains quotation marks 
	i = argc - 1;
	if(strlen(argcv[i]) > 0 && argcv[i][strlen(argcv[i]) - 1] == '"')
	{
		message[size - 2] = '\0';
	}

	//Initialize the signal handler
	Signal(SIGALRM, handler);
	
	//Fork the parent process to create a child process
	pid_t pid; 
	if ((pid =Fork()) == 0)
	{
		//Set an alarm for the time duration given by the user
		Alarm(time);
		//Pause the process till the signal is recieved
		Pause();
		//Then exit the child process
		exit(1);
        }

   //Print the id of the child process stored in pid
   printf("%s process-id1 %d\n", argcv[0], pid);
   
   //Free memory allocated for the message pointer
   free(message);
   message = NULL;
  
  return 0;
}
