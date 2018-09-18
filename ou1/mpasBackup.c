#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

/*
 * Function:  main
 * --------------------
 * s
 * The program takes a textfile of user-accounts,
 * append their repectivly UID:s and usernames to a linked list.
 * The program sorts the list and prints the accounts.
 * The textfile can be imported via inputparameter aswell as stdin.
 *
 * Example execution:	./mpasswdsort infile
 * 						./mpasswwdsort < infile
 *
 * Example output:
 * 						1171:mr
 *						12037:set
 *						12856:axelsson
 *						12928:dahlin
 *						16928:gabriel
 *						17847:fahlgren
 *
 *  argc:		number input-parameters
 * 	argv[]:		Char* pointing to a char-array of input-parameters
 *
 *  returns:	EXIT_FAILURE if something goes wrong or a text-row is invalid.
 * 				EXIT_SUCCESS if execution went succesfully.
 */
int main(int argc, char *argv[])
{

	node *list = linkedList_create();
	FILE *fp = NULL;

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
		linkedList_free(list);
		exit(EXIT_FAILURE);
	}

	if (fp == NULL)
	{
		fprintf(stderr, "ERROR: Could not open file\n");
		linkedList_free(list);
		exit(EXIT_FAILURE);
	}
	else
	{
		int lineNumber = 1;
		char *line;
		size_t bufsize = 1024;
		bool reachedEOF = false;

		line = malloc(bufsize * sizeof(char));
		if (line == NULL)
		{
			fprintf(stderr, "ERROR: Unable to allocate buffer\n");
			linkedList_free(list);
			exit(1);
		}

		while (!reachedEOF)
		{
			if (getline(&line, &bufsize, fp) != -1)
			{
				bool faulthy_row = false;
				bool empty_row = false;

				int countingSemiColons = 0;
				for (unsigned int i = 0; i < strlen(line); i++) // Replace ":" with " "
				{
					if (line[i] == ':')
					{
						countingSemiColons++;
					}
				}

				line[strlen(line) - 1] = 0; // Remove last /n-char

				char *UID = calloc(bufsize, sizeof(char));
				if (UID == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					exit(1);
				}

				char *username = calloc((bufsize),sizeof(char));
				if (username == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					exit(1);
				}

				char *password = calloc(bufsize, sizeof(char));
				if (password == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					free(username);
					exit(1);
				}

				char *GID = calloc(bufsize, sizeof(char));
				if (GID == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					free(username);
					free(password);
					exit(1);
				}

				char *GECOS = calloc(bufsize,sizeof(char));
				if (GECOS == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					free(username);
					free(password);
					free(GID);
					exit(1);
				}

				char *directory = calloc(bufsize, sizeof(char));
				if (directory == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					free(username);
					free(password);
					free(GID);
					free(GECOS);
					exit(1);
				}

				char *shell = calloc(bufsize, sizeof(char));
				if (shell == NULL) // Checking allocated memory for NULL-pointers
				{
					fprintf(stderr, "ERROR: Unable to allocate memory\n");
					free(line);
					free(UID);
					free(username);
					free(password);
					free(GID);
					free(GECOS);
					free(directory);
					exit(1);
				}

				int workingOnFieldNr = 0;

				int usernameLength = 0;
				int passwordLength = 0;
				int UIDLength = 0;
				int GIDLength = 0;
				int GECOSLength = 0;
				int directoryLength = 0;
				int shellLength = 0;

				for(unsigned int j = 0; j<strlen(line); j++){

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
							fprintf(stderr, "Line %d: Encountered too many semicolons. Invalid format.\n", lineNumber);
							faulthy_row = true;
							free(username);
						}
					}
				}


				// CHECK STUFF
				char *checkGIDNumeric;
				strtol(GID, &checkGIDNumeric, 10);

				if (strlen(line) == 0)
				{
					fprintf(stderr, "Line %d: Encountered a <BLANKLINE>\n", lineNumber);
					empty_row = true;
					//free(username);
				}
				else if (strlen(checkGIDNumeric) != 0)
				{
					fprintf(stderr, "Line %d: The 'gid' field has to be numeric: %s\n", lineNumber,line);
					faulthy_row = true;
					free(username);
				}
				else if(countingSemiColons != 6)
				{
					fprintf(stderr, "Line %d: Found %d semicolons, 6 is required. Invalid format.\n", lineNumber, countingSemiColons);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(username) == 0 || strlen(username) > 32)
				{
					fprintf(stderr, "Line %d: Username is invalid. Must be 1-32 charachers.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(UID) == 0)
				{
					fprintf(stderr, "Line %d: Missing UID, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(GID) == 0)
				{
					fprintf(stderr, "Line %d: Missing GID, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(directory) == 0)
				{
					fprintf(stderr, "Line %d: Missing directory, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if (strlen(shell) == 0)
				{
					fprintf(stderr, "Line %d: Missing shell, invalid format.\n", lineNumber);
					faulthy_row = true;
					free(username);
				}
				else if(GID[0] == '-')
				{
					fprintf(stderr, "Line %d: GID has to be a positive number. Got \"%s\" \n", lineNumber, GID);
					faulthy_row = true;
					free(username);
				}
				else if (UID[0] == '-')
				{
					fprintf(stderr, "Line %d: UID has to be a positive number. Got \"%s\" \n", lineNumber, UID);
					faulthy_row = true;
					free(username);
				}



				if(!faulthy_row && !empty_row){ // Append to list if the row is legit

					char *ptr;

					// ADD TO LIST
					user_info *newValue = malloc(sizeof(user_info));
					if (newValue == NULL)
					{
						fprintf(stderr, "ERROR: Unable to allocate memory\n");
						exit(EXIT_FAILURE);
  					}

					newValue->uid = strtol(UID,&ptr,10);
					newValue->uname = username;


					if(strlen(ptr) == 0){
						linkedList_appendUserInfo(list, newValue);
					}else{

						fprintf(stderr, "Line %d: UID has to be a number. Got: \"%s\n", lineNumber, UID);
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

