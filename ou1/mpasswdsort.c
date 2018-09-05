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
		int characters;
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
				char *username = malloc(bufsize * sizeof(char));
				
				char password[bufsize];
				char GID[bufsize];
				char GECOS[bufsize];
				char directory[bufsize];
				char shell[bufsize];

				sscanf(line, "%s %s %s %s %s %s %s", username, &password, UID, &GID, &GECOS, &directory, &shell);

				// CHECK STUFF

				if (strlen(username) == 0)
				{
					printf("ERROR: Line %d is faulthy\n",lineNumber);
					faulthy_row = true;
				}
				else if (strlen(password) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}
				else if (strlen(UID) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}
				else if (strlen(GID) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}
				else if (strlen(GECOS) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}
				else if (strlen(directory) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}
				else if (strlen(shell) == 0)
				{
					printf("ERROR: Line %d is faulthy\n", lineNumber);
					faulthy_row = true;
				}

				if(!faulthy_row){ // Append to list if the row is legit

					char *ptr;

					// ADD TO LIST
					user_info *newValue = malloc(sizeof(user_info));
					newValue->uid = strtol(UID,&ptr,10);
					newValue->uname = username;

					linkedList_append(list, newValue);

				}

				free(UID);

				/*
				free(password);
				free(GID);
				free(GECOS);
				free(directory);
				free(shell);
				*/
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

	linkedList_print(list);
	printf("-- SORTING --\n");
	linkedList_sort(list);
	linkedList_print(list);


	linkedList_free(list);
	return 0;
}
