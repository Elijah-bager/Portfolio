/*DO NOT REMOVE OR CHANGE THIS COMMENT:CSE 2431 Lab 3 AU 25 052121*/

/* STUDENT NAME:  Elijah Baugher   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXLINE 40 /* 40 chars per line, per command, is enough. */

/* function to add last user entered command to historyBuff, 	*/
/* if it is not an h/history command, or a rerun command. 	*/
/* Notice that the function must check if fewer than 6 commands */
/* have been run, which means the command should be added as    */
/* the last command, or if 6 or more commands have been run     */
/* which means that the commands in positions 2 through 6 in    */
/* historyBuff should all be moved up one spot: i.e., command 2 */
/* should be moved to command 1; command 3 should be moved to   */
/* command 2; command 4 should be moved to command 3; command 5 */
/* should be moved to command 4, command 6 should be moved to 5,*/
/* and the command last entered should be put in as command 6.  */

void updateHistory (char *command, int numCommands, char *commandPtrs[] ) 
{
    if (numCommands <= 6) {
        strcpy(commandPtrs[numCommands], command);
    } else {
        for (int i = 1; i < 6; i++) {
            strcpy(commandPtrs[i], commandPtrs[i + 1]);
        }
        strcpy(commandPtrs[6], command);
    }
}

/* function that prints out commands in the current history 	*/
/* buffer - up to the 6 most recent commands which are not 	*/
/* h/history or a rerun command. The commands are numbered 	*/
/* when printed, from 1), the least recently run command, to 	*/
/* 6) (if 6 or more commands have been entered and run),   	*/
/* which is the most recently run command. If fewer than 6 	*/
/* commands have been entered and run so far, then the     	*/
/* commands in the list are numbered from 1), the least     	*/
/* recently run command, up to the number of commands run,	*/
/* with the last numbered command being the most recently run.  */
/* For example, if 3 commands have been run, the commands  	*/
/* will be numbered from 1 to 3, with the command numbered 	*/
/* 1 being the least recently run, and the command numbered     */
/* 3 being the most recently run command.                   	*/

void printHistory (int numCommands, char *historyBuff) 
{

    for (int i = 0; i < numCommands && i < 6; i++) {
        printf("%d) %s \n", i + 1, &historyBuff[i * MAXLINE]);
    }
}

/* function which determines if a command it is passed is 	*/
/* an h/history command, and if so, returns 1, else returns 0; 	*/
/* int return value of function is used as a Boolean.		*/
/* Use strcmp to determine if command is h or history.          */

int isHistory ( char *args[] ) 
{	
	if (args[0] != NULL) {
        if (strcmp(args[0], "h") == 0 || strcmp(args[0], "history") == 0) {
            return 1;
        }
    }
  return 0; /* write/modify the code to return either */
            /* 0 or 1, as apprpriate   */
}

/* function which determines if a command it is passed is 	*/
/* a rerun command, and if so, returns 1, else returns 0; 	*/
/* int retrun value of function is used as a Boolean.		*/
/* Use strcmp for this function also.                           */


int isRerun ( char *args[] ) 
{
	if (args[0] != NULL) {
        if (strcmp(args[0], "rr") == 0 || args[0][0] == 'r') {
            return 1;
        }
    }
  return 0; /* write/modify the code to return either */
            /* 0 or 1, as apprpriate   */
}

/* function which is passed rerun command and returns index of	*/
/* command to rerun. This requires numCommands also. For	*/
/* For example, if numCommands is 3, and the rerun command is   */
/* rr, the function returns 3. If, however, for example,        */
/* numCommands is greater than or equal to 5, and the command is*/
/* rr, the function returns 5. As another example, if the rerun */
/* command is r3, the function returns 3, and so on.   */

int rerunIndex ( char *args[], int numCommands ) 
{
    if (strcmp(args[0], "rr") == 0) {
        return (numCommands < 5) ? numCommands : 5;
    } else if (args[0][0] == 'r') {
        int index = atoi(&args[0][1]);
        if (index >= 1 && index <= 5) {
            return index;
        }
    }
    /* if no valid rr or rN form was provided, return -1 to indicate error */
    return -1;
}

/** The setup() function reads in the next command line string 	*/
/* storing it in the input buffer. The line is separated into	*/
/* distinct tokens using whitespace as delimiters.  Setup also 	*/
/* modifies the args parameter so that it holds pointers to the */
/* null-terminated strings  which are the tokens in the most	*/
/* recent user command as well as a NULL pointer, 		*/
/* indicating the end of the argument list, which comes after	*/ 
/* the string pointers that have been assigned to args.		*/

void setup(char inputBuff[], char *args[],int *background, char commandCopy[], int rerun)
{
    int length,  /* Num characters in the command line, including newline character */
        i,       /* Index for inputBuff arrray          */
        j,       /* Where to place the next parameter into args[] */
        start;   /* Beginning of next command parameter */

/* Use the rerun parameter as a Boolean to control whether the 	*/ 
/* setup function code will read the command the user entered, 	*/
/* put a null byte at the end, copy the command into commandCopy, */
/* and then parse the command in an args array, OR whether the	*/
/* setup function will copy what is in commandCopy into 	*/
/* inputBuff, and then allow the parse code to parse it into an	*/ 
/* args array, so that the command can be rerun by the code in	*/
/* main after the child is forked.  				*/      


    /* If rerun is 0: 1) Read what the user entered, 2) Put a null */
    /* byte at the end of the command using using length, and then */
    /* 3) Use strcpy to copy what is in inputBuff into commandCopy,*/
    /* DO THE ABOVE BEFORE the code below parses what is in        */
    /* inputBuff into an args array.     */

    if (rerun == 0) {
     length = read(STDIN_FILENO, inputBuff, MAXLINE);
     /* Place a null byte/character at the end of inputBuff, after the new line */
     if (length > 0) {
          if (length < MAXLINE) inputBuff[length] = '\0';
          else inputBuff[MAXLINE-1] = '\0';

          /* Copy the full command into commandCopy so the main loop can
              add it to the history buffer later */
          strcpy(commandCopy, inputBuff);
     }

     }

    /* else case: if rerun is 1, then copy what is in commandCopy into inputBuff, */
    /* so that the code below can parse it into an args array, which is needed to */
    /* be able to echo the command and rerun the command in main after forking a  */
    /* child. In this case, length also has to be assigned the correct value; use */      
    /* strlen with the string inputBuff.				          */
    else {

    strcpy(inputBuff, commandCopy);
    length = strlen(inputBuff);

    }

    /* ****    DO NOT MODIFY THE setup CODE BELOW    **** */
    j = 0;
    start = -1;

    if (length == 0)
        exit(0);            /* Cntrl-d was entered, end of user command stream */

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }
    
    /* Examine every character in the input buffer */

    for (i = 0; i < length; i++) {
 
        switch (inputBuff[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &inputBuff[start];    /* Set up pointer */
                j++;
            }

            inputBuff[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &inputBuff[start];     
                j++;
            }

            inputBuff[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *background = 1;
            inputBuff[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 50 */
} 

int main(void)
{
    char inputBuff[MAXLINE]; /* Input buffer  to hold the command entered */
    char *args[MAXLINE/2+1];/* Command line arguments */
    int background;         /* Equals 1 if a command is followed by '&', else 0 */
    char commandCopy[MAXLINE]; /* Array to store copy of command in setup function */
    char historyBuff[6*MAXLINE];  /* char buffer to hold up to 6 of the most */
				  /* recently entered commands */
    char *commandPtrs[7] = {NULL, &historyBuff[0], &historyBuff[40], &historyBuff[80],
				&historyBuff[120], &historyBuff[160], &historyBuff[200]};	
						/* array of pointers to each of the 	*/
						/* commands in the historyBuff. For 	*/
						/* example, commandPtrs[1] points to 	*/
						/* the string for the command numbered 1 */
						/* in the historyBuff, ...., and 	*/
						/* commandPtrs[6] points to the string   */
						/* for the command numbered 6 in the 	*/
						/* historyBuff (if there is such a	*/
						/* command stored, which can be checked	*/
						/* with numCommands. These pointers can */
						/* be used to copy a command from one	*/
						/* position in the buffer to another,	*/
						/* or to pass a command to the setup 	*/
						/* function to rerun it, for example.	*/

    int numCommands = 0;	/* variable to track the number of commands the user   */
				/* has entered so far. It is incremented for any       */
				/* command other than a history or rerun command.      */

    int rerun;			/* Boolean to track whether the command entered by the */
				/* user was a rerun command or not. If it was a rerun  */
				/* command, setup must be called a second time; see    */
				/* below.      */
    

    while (1)
    {    
	background = 0;
	pid_t pid;

	printf("CSE2431Shell$ ");  /* Shell prompt */
        fflush(0);

	rerun = 0; /* reset rerun to 0 before 1st call to setup */

        setup(inputBuff, args, &background, commandCopy, rerun);  /* Get next command */
  	
	/* We DO NOT fork a child if the commamnd read by setup is h/history!! */
	if (isHistory (args) ) {
	  printHistory (numCommands, historyBuff);
	} /* end if */

	/* if command is not h/history command execute else code below */
	else {
		rerun = isRerun (args); /* set rerun Boolean appropriately */
		if (rerun) {
		    /* call printf to echo command to be rerun */
		    printf("%s",commandPtrs[rerunIndex( args, numCommands )]);
			/* if command is rerun command, call setup 2nd time to parse 	*/
			/* command from historyBuff and get args array to rerun command	*/
			setup(inputBuff, args, &background, 
			   commandPtrs[rerunIndex( args, numCommands )], rerun); 
		} /* end if */
		else {
            /* if command is neither h/history nor rerun, increment		*/
            /* numCommands and call function to add command to historyBuff.	*/
            numCommands += 1;
            updateHistory ( commandCopy, numCommands, commandPtrs );
		} /* end else */

		pid = fork();
	 
 		if (pid < 0) {
	    		printf ("ERROR: fork failed!\n");
	   		printf("Terminating shell\n");
	    		exit (0);
		} /* end if */
        	else if (pid == 0) {
			execvp(args[0], args);
		} /* end else if */
		else {
			if (background == 0) waitpid(pid , NULL, 0);
        	} /* end else */
	} /* end else */
   } /* end while */
} /* end main */

