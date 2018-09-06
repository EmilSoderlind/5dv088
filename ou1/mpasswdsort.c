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

	if (argc == 2) // If we got input-parameters
	{
		fp = fopen(argv[1], "r");
	}
	else if (argc == 1) // Check STDIN
	{ 
		fp = stdin;
	}
	else if (argc > 2) // Too many input-parameters
	{ 
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

				int countingSemiColons = 0;
				for (int i = 0; i < strlen(line); i++) // Replace ":" with " "
				{
					if (line[i] == ':')
					{
						countingSemiColons++;
					}
				}

				line[strlen(line) - 1] = 0; // Remove last /n-char
		
				char *UID = calloc(bufsize, sizeof(char));
				char *username = calloc((bufsize+1),sizeof(char));

				char *password = calloc(bufsize, sizeof(char));
				char *GID = calloc(bufsize, sizeof(char));
				char *GECOS = calloc(bufsize,sizeof(char));
				char *directory = calloc(bufsize, sizeof(char));
				char *shell = calloc(bufsize, sizeof(char));

				//sscanf(line, "%s %s %s %s %s %s %s", username, password, UID, GID, GECOS, directory, shell);
				
				int workingOnFieldNr = 0;

				int usernameLength = 0;
				int passwordLength = 0;
				int UIDLength = 0;
				int GIDLength = 0;
				int GECOSLength = 0;
				int directoryLength = 0;
				int shellLength = 0;

				for(int j = 0; j<strlen(line); j++){

					if(line[j] == ':'){
						workingOnFieldNr++;
					}else{

						switch (workingOnFieldNr)
						{
						case 0:
							username[usernameLength] = line[j];
							usernameLength++;
							break; 
						case 1:
							password[passwordLength] = line[j];
							passwordLength++;
							break;
						case 2:
							UID[UIDLength] = line[j];
							UIDLength++;
							break;
						case 3:
							GID[GIDLength] = line[j];
							GIDLength++;
							break;
						case 4:
							GECOS[GECOSLength] = line[j];
							GECOSLength++;
							break;
						case 5:
							directory[directoryLength] = line[j];
							directoryLength++;
							break;
						case 6:
							shell[shellLength] = line[j];
							shellLength++;
							break;
						default:
							printf("Line %d: Encountered too many semicolons. Invalid format.\n", lineNumber);
							faulthy_row = true;
							free(username);
						}
					}
				}

				/*
				printf("\n---\n");
				printf("Username: %s\n",username);
				printf("password: %s\n", password);
				printf("UID: %s\n", UID);
				printf("GID: %s\n", GID);
				printf("GECOS: %s\n", GECOS);
				printf("directory: %s\n", directory);
				printf("shell: %s\n", shell);
				printf("\n---\n");
				*/

				// CHECK STUFF

				if (strlen(line) == 0)
				{
					printf("Line %d: Encountered a <BLANKLINE>\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if(countingSemiColons != 6)
				{
					printf("Line %d: Found %d semicolons, 6 is required. Invalid format.\n", lineNumber,countingSemiColons);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(username) == 0 || strlen(username) > 32)
				{
					printf("Line %d: Username is invalid. Must be 1-32 charachers.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(UID) == 0)
				{
					printf("Line %d: Missing UID, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(GID) == 0)
				{
					printf("Line %d: Missing GID, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(directory) == 0)
				{
					printf("Line %d: Missing directory, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(shell) == 0)
				{
					printf("Line %d: Missing shell, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if(GID[0] == '-')
				{
					printf("Line %d: GID has to be a positive number. Got \"%s\" \n", lineNumber,GID);
					faulthy_row = true;
					free(username);
				}
				else if (UID[0] == '-')
				{
					printf("Line %d: UID has to be a positive number. Got \"%s\" \n", lineNumber, UID);
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

