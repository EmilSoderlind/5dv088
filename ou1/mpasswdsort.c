#include "list.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{

	node *unsortedList = linkedList_create();
	node *sortedList = linkedList_create();

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
		int lineNumber = 0;
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
			{ // EOF
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

				char *username = malloc(bufsize * sizeof(char));
				char *password = malloc(bufsize * sizeof(char));
				char *UID = malloc(bufsize * sizeof(char));
				char *GID = malloc(bufsize * sizeof(char));
				char *GECOS = malloc(bufsize * sizeof(char));
				char *directory = malloc(bufsize * sizeof(char));
				char *shell = malloc(bufsize * sizeof(char));


				sscanf(line, "%s %s %s %s %s %s %s", username, password, UID, GID, GECOS, directory, shell);

				// CHECK STUFF
				/*
				printf("username: %s ", username);
				printf("password: %s ", password);
				printf("UID: %s ", UID);
				printf("GID: %s ", GID);
				printf("GECOS: %s ", GECOS);
				printf("directory: %s ", directory);
				printf("shell: %s\n", shell);
				*/

				char *ptr;

				// ADD TO LIST
				user_info *newValue = malloc(sizeof(user_info));
				newValue->uid = strtol(UID,&ptr,10);
				newValue->uname = username;

				linkedList_append(unsortedList, newValue);

				free(password);
				free(UID);
				free(GID);
				free(GECOS);
				free(directory);
				free(shell);
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

	




	/*
	node *rootNode = linkedList_create();

	user_info *value = malloc(sizeof(user_info));
	value->uid = 3;
	value->uname = "Emil";

	user_info *valueTwo = malloc(sizeof(user_info));
	valueTwo->uid = 24;
	valueTwo->uname = "Simon";

	linkedList_append(rootNode, value);
	linkedList_append(rootNode, valueTwo);


	linkedList_free(rootNode);*/

	return 0;

	linkedList_print(unsortedList);
	linkedList_free(unsortedList);
}
