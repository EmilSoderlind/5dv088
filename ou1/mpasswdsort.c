#include "list.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{

	node *list = linkedList_create();

	FILE *fp;

	bool foundFaultyRow = false;

	if (argc == 2) // OM vi har input-parameters ta hand om dom
	{
		// Om filen fuckar --> felmeddelande
		fp = fopen(argv[1], "r");
	
	}
	else if (argc == 1) // Check STDIN
	{ 
		fp = stdin;
	}
	else if (argc > 2)
	{ // OM vi har för många input-parameters --> felmeddelande
		fprintf(stderr, "ERROR: To many parameters\n");
		exit(EXIT_FAILURE);
	}
	
	if (fp == NULL)
	{
		fprintf(stderr, "ERROR: Could not open file\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		int lineNumber = 1;
		char *line;
		size_t bufsize = 1023;
		bool reachedEOF = false;

		line = malloc(bufsize * sizeof(char));
		if (line == NULL)
		{
			fprintf(stderr, "ERROR: Unable to allocate buffer\n");
			exit(1);
		}

		while (!reachedEOF)
		{
			if (getline(&line, &bufsize, fp) != -1)
			{
				bool faulthy_row = false;

				line[strlen(line) - 1] = 0; // Remove last /n-char

				for(int i = 0; i<strlen(line);i++){ // Replace " " with _
					if(line[i] == ' '){
						line[i] = '_';
					}
				}

				for (int i = 0; i < strlen(line); i++) // Replace ":" with " "
				{
					if (line[i] == ':')
					{
						line[i] = ' ';
					}
				}

				char *UID = malloc(bufsize * sizeof(char));
				char *username = malloc((bufsize+1) * sizeof(char));

				char *password = malloc(bufsize * sizeof(char));
				char *GID = malloc(bufsize * sizeof(char));
				char *GECOS = malloc(bufsize * sizeof(char));
				char *directory = malloc(bufsize * sizeof(char));
				char *shell = malloc(bufsize * sizeof(char));

				sscanf(line, "%s %s %s %s %s %s %s", username, password, UID, GID, GECOS, directory, shell);

				// CHECK STUFF

				if (strlen(line) == 0)
				{
					printf("Line %d: Encountered a <BLANKLINE>\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(username) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(password) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(UID) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(GID) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(GECOS) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(directory) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(shell) == 0)
				{
					printf("Line %d: Invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}else if(GID[0] == '-')
				{
					printf("Line %d: GID has to be a positive number. Got \"%s\" \n", lineNumber,GID);
					faulthy_row = true;
					free(username);
				}

				if(!faulthy_row){ // Append to list if the row is legit

					char *ptr;

					// ADD TO LIST
					user_info *newValue = malloc(sizeof(user_info));
					newValue->uid = strtol(UID,&ptr,10);
					newValue->uname = username;


					if(strlen(ptr) == 0){
						linkedList_append(list, newValue);
					}else{

						printf("Line %d: UID has to be a number. Got: \"%s\n", lineNumber,UID);
						faulthy_row = true;
						free(username);
						free(newValue);
					}

				}

				free(UID);
				free(password);
				free(GID);
				free(GECOS);
				free(directory);
				free(shell);

				if (!foundFaultyRow && faulthy_row)
				{
					foundFaultyRow = true;
				}
			}
			else
			{
				reachedEOF = true;
			}
			lineNumber++;

		}

		free(line);
	}
	fclose(fp);

	linkedList_sort(list);
	
	linkedList_print(list);

	linkedList_free(list);


	if(foundFaultyRow){
		return EXIT_FAILURE;
	}else{
		return EXIT_SUCCESS;
	}
}
