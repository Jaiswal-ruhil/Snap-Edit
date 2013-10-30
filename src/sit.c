/** 
*  	 AUTHOR       - Ravi Kumar L
*  	 email        - upman16@gmail.com
*  	 Program Name - Snap-Edit (Sit)
*        Language     - C
*  	 Description:   A simple Line-Editor written in C with no
*                       heed to efficiency.     
*        
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


// Add functionality to offer to save current buffer before exit -- Done

#define HANDLE_CALLOC(p,x,y) 				\
	if (!(p=calloc(x,y)))  			\
	{ 						\
		printf("\n\nInsufficient Memory!"); 	\
		exit(EXIT_FAILURE); 			\
	}

#define HANDLE_REALLOC(p,x) \
				\
	if (!(p=realloc(p,x))) \
	{	\
		printf("\n\nInsufficeint Memory"); \
		exit(EXIT_FAILURE); 	\
	}

#include "sit.h"


//Function declarations
void displayhelp(); 
int write(FILE **); //Writes buffer onto file
int print(); // Prints contents of the buffer onto the concole 
int append(int, char *); // Appends the array at the char * to the passed line
int replace(int , char *); //replaces the content of the passed line number with the passed string 
int delete(int); //free()'s the structure and string of the line number passed and changes the line numbers on the following line structures
void expandstring(line *); //expands the string the line to which pointer is passed
void allocstring(line *); // Allocates string on the pointer to line passed
void buildbuffertill(int);// builds the line structure till the line number passed
line * findline(int); // returns a pointer to the line structure of the line number passed
void processraw(char *,char *); //processes the raw input command string received and identifies command and line number and copies input string if  any into the second pointer to string passed
void displayhelp();
void readfileintobuffer(FILE *);




int main(int argc, char *argv[])
{
	
	//File Creation
	//add edit already existing file funcionality -- Done
	FILE *fptr;
	char *string=(char *)calloc(sizeof(char),500);
	char *rawin=(char *)calloc(sizeof(char),500);
	
	//create buffer and build first line
	HANDLE_CALLOC(buffer,sizeof(line),1);
	lastline=buffer;
	lastline->thisline=1;
	
	if (argc==1) //no filename given by the user
		printf("\nUsage: ./sit filename\n\n"),exit(1);
	if(argc>2) //Too many arguments enetred by the user
		printf("\nToo many arguments\n\n");
	
	else if(argc==2) //filename received
	{	strcpy(filename,argv[1]);
		if(fptr= (FILE *)fopen(filename,"r")) //check if file with same name exists
			readfileintobuffer(fptr);
		else
		{
			if(! (fptr = (FILE *)fopen(filename,"w")))//open file and check if successful
				printf("Memory error"), exit(1);		

		}
			

	}

	
	


	displayhelp();


	//Ready to receive commands
	while(1)
	{	
			
		printf("\n?\n");
		fflush(stdin);
		gets(rawin);
		processraw(rawin,string);
		

		
		switch (command)
		{
			case QUIT: 	if(savebeforequit)
					{
						char c;
						printf("The buffer has been changed since the last save,  save now? \n[y or n]? :\t");

						scanf("%c",&c);
						if(c=='y' || c=='Y')
							write(&fptr);
					}
					exit (0);
					break;
				   		
			case REPLACE:   replace(linenum,string);
					break;
			case APPEND:    append(linenum,string);
					break;
			case WRITE:     write(&fptr);
					break;
			case PRINT:     print();
					break;
			case DELETE:    if(delete(linenum)==LINE_ERROR)
						printf("Invalid line number");
					break;
			case COMMAND_ERROR: printf("Command Error, type 'help' for help");
						break;
			case LINE_ERROR: printf("Improper line number");
					break;
			case HELP: displayhelp();
					break;
			default: printf("Command Error");
 			 		

		}
		
		command = 0;
		linenum =0;
		memset(rawin,'\0',500);
		memset(string,'\0',500);
		
		
			
	}
	
}
