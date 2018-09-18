#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

/*
 * Function:  appendToList(char *UID, bool *faulthy_row, int lineNumber, node *list, char *username)
 * --------------------
 *
 *  Append username and UID to linkedList
 * 
 *  UID:				UID-variable
 *  username: 			username-variable
 *  faulthy_row:		bool-flag to save if faulthy row is found
 *  lineNumber: 		Current lines index
 *  list: 				LinkedList
 */
static void appendToList(char *UID, bool *faulthy_row, int lineNumber, node *list, char *username)
{
	char *ptr;

	// ADD TO LIST
	user_info *newValue = malloc(sizeof(user_info));
	if (newValue == NULL)
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	newValue->uid = strtol(UID, &ptr, 10);
	newValue->uname = username;

	if (strlen(ptr) == 0)
	{
		linkedList_appendUserInfo(list, newValue);
	}
	else
	{

		fprintf(stderr, "Line %d: UID has to be a number. Got: \"%s\n", lineNumber, UID);
		*faulthy_row = true;
		free(username);
		free(newValue);
	}
}

/*
 * Function:  allocateParameters(char **GECOS, char **GID, char **UID, size_t bufsize, char **directory, char *line, char **password, char **shell, char **username)
 * --------------------
 *
 *  Allocate memory to parameters to be parsed fron line
 * 
 *  GECOS:			GECOS-variable
 *  GID:			GID-variable
 *  UID:			UID-variable
 *  directory: 		directory-variable
 *  shell: 			shell-variable
 *  username: 		username-variable
 *  bufsize:		Max length of line
 *  line:			Line (String) to be filled from textfile-line
 */
static void allocateParameters(char **GECOS, char **GID, char **UID, size_t bufsize, char **directory, char *line, char **password, char **shell, char **username)
{
	*UID = calloc(bufsize, sizeof(char));
	if (*UID == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		exit(1);
	}

	*username = calloc((bufsize), sizeof(char));
	if (*username == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		exit(1);
	}

	*password = calloc(bufsize, sizeof(char));
	if (*password == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		free(*username);
		exit(1);
	}

	*GID = calloc(bufsize, sizeof(char));
	if (*GID == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		free(*username);
		free(*password);
		exit(1);
	}

	*GECOS = calloc(bufsize, sizeof(char));
	if (*GECOS == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		free(*username);
		free(*password);
		free(*GID);
		exit(1);
	}

	*directory = calloc(bufsize, sizeof(char));
	if (*directory == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		free(*username);
		free(*password);
		free(*GID);
		free(*GECOS);
		exit(1);
	}

	*shell = calloc(bufsize, sizeof(char));
	if (*shell == NULL) // Checking allocated memory for NULL-pointers
	{
		fprintf(stderr, "ERROR: Unable to allocate memory\n");
		free(line);
		free(*UID);
		free(*username);
		free(*password);
		free(*GID);
		free(*GECOS);
		free(*directory);
		exit(1);
	}
}

/*
 * Function:  checkParameters(char *GID, char *UID, int countingSemiColons, char *directory, bool *empty_row, bool *faulthy_row, char *line, int lineNumber, char *shell, char *username)
 * --------------------
 *
 *  Check parsed parameters for invalidness
 * 
 *  password: 			password-variable
 *  GID:				GID-variable
 *  UID:				UID-variable
 *  directory: 			directory-variable
 *  shell: 				shell-variable
 *  username: 			username-variable
 *  faulthy_row:		bool-flag to save if faulthy row is found
 *  empty_row:			bool-flag to save if empty row is found
 *  line:				Current line
 *  lineNumber: 		Current lines index
 *  countingSemiColons:	Index to count semicolons
 */
static void checkParameters(char *GID, char *UID, int countingSemiColons, char *directory, bool *empty_row, bool *faulthy_row, char *line, int lineNumber, char *shell, char *username)
{
	char *checkGIDNumeric;
	strtol(GID, &checkGIDNumeric, 10);

	if (strlen(line) == 0)
	{
		fprintf(stderr, "Line %d: Encountered a <BLANKLINE>\n", lineNumber);
		*empty_row = true;
		//free(username);
	}
	else if (strlen(checkGIDNumeric) != 0)
	{
		fprintf(stderr, "Line %d: The 'gid' field has to be numeric: %s\n", lineNumber, line);
		*faulthy_row = true;
		free(username);
	}
	else if (countingSemiColons != 6)
	{
		fprintf(stderr, "Line %d: Found %d semicolons, 6 is required. Invalid format.\n", lineNumber, countingSemiColons);
		*faulthy_row = true;
		free(username);
	}
	else if (strlen(username) == 0 || strlen(username) > 32)
	{
		fprintf(stderr, "Line %d: Username is invalid. Must be 1-32 charachers.\n", lineNumber);
		*faulthy_row = true;
		free(username);
	}
	else if (strlen(UID) == 0)
	{
		fprintf(stderr, "Line %d: Missing UID, invalid format.\n", lineNumber);
		*faulthy_row = true;
		free(username);
	}
	else if (strlen(GID) == 0)
	{
		fprintf(stderr, "Line %d: Missing GID, invalid format.\n", lineNumber);
		*faulthy_row = true;
		free(username);
	}
	else if (strlen(directory) == 0)
	{
		fprintf(stderr, "Line %d: Missing directory, invalid format.\n", lineNumber);
		*faulthy_row = true;
		free(username);
	}
	else if (strlen(shell) == 0)
	{
		fprintf(stderr, "Line %d: Missing shell, invalid format.\n", lineNumber);
		*faulthy_row = true;
		free(username);
	}
	else if (GID[0] == '-')
	{
		fprintf(stderr, "Line %d: GID has to be a positive number. Got \"%s\" \n", lineNumber, GID);
		*faulthy_row = true;
		free(username);
	}
	else if (UID[0] == '-')
	{
		fprintf(stderr, "Line %d: UID has to be a positive number. Got \"%s\" \n", lineNumber, UID);
		*faulthy_row = true;
		free(username);
	}
}

/*
 * Function:  fillParameters(char *GECOS, char *GID, char *UID, char *directory, bool *faulthy_row, char *line, int lineNumber, char *password, char *shell, char *username)
 * --------------------
 *
 *  Parse parameters from line, char by char.
 * 
 *  GECOS:			GECOS-variable
 *  GID:			GID-variable
 *  UID:			UID-variable
 *  directory: 		directory-variable
 *  password: 		password-variable
 *  shell: 			shell-variable
 *  username: 		username-variable
 *  faulthy_row:	bool-flag to save if faulthy row is found
 *  line:			Current line
 *  lineNumber: 	Current lines index
 */
static void fillParameters(char *GECOS, char *GID, char *UID, char *directory, bool *faulthy_row, char *line, int lineNumber, char *password, char *shell, char *username)
{
	int workingOnFieldNr = 0;

	int usernameLength = 0;
	int passwordLength = 0;
	int UIDLength = 0;
	int GIDLength = 0;
	int GECOSLength = 0;
	int directoryLength = 0;
	int shellLength = 0;

	for (unsigned int j = 0; j < strlen(line); j++)
	{

		if (line[j] == ':')
		{
			workingOnFieldNr++;
		}
		else
		{

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
				*faulthy_row = true;
				free(username);
			}
		}
	}
}

/*
 * Function:  parseLine(size_t bufsize, bool *foundFaultyRow, char *line, int lineNumber, node *list)
 * --------------------
 *
 *  Parse parameters from textfile-line and apend to linkedList
 * 
 *  bufsize:		Maximum length of line
 *  foundFaultyRow:	bool-flag to save if faulthy row is found
 *  line:			Current line as String
 *  lineNumber: 	Current lines index
 *  list:			Linked list to save User-info in
 */
static void parseLine(size_t bufsize, bool *foundFaultyRow, char *line, int lineNumber, node *list)
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

	char *UID;
	char *username;
	char *password;
	char *GID;
	char *GECOS;
	char *directory;
	char *shell;

	allocateParameters(&GECOS, &GID, &UID, bufsize, &directory, line, &password, &shell, &username);

	fillParameters(GECOS, GID, UID, directory, &faulthy_row, line, lineNumber, password, shell, username);

	// CHECK STUFF
	checkParameters(GID, UID, countingSemiColons, directory, &empty_row, &faulthy_row, line, lineNumber, shell, username);

	if (!faulthy_row && !empty_row)
	{ // Append to list if the row is legit
		appendToList(UID, &faulthy_row, lineNumber, list, username);
	}

	free(UID);
	free(password);
	free(GID);
	free(GECOS);
	free(directory);
	free(shell);

	if (!*foundFaultyRow && faulthy_row)
	{
		*foundFaultyRow = true;
	}
}

/*
 * Function:  openFileInput(int argc, char **argv, FILE **fp)
 * --------------------
 *
 *  Assign filepath to fp, stdin or external file given as input-parameter
 * 
 *  argc:	Number of input-arguments to main-function
 *  argv:   Input-arguments to main-function
 *  fp:		FILE to be assigned to stdin or file
 */
static void openFileInput(int argc, char **argv, FILE **fp)
{
	if (argc == 2) // If we got input-parameters
	{
		*fp = fopen(argv[1], "r");
	}
	else if (argc == 1) // Check STDIN
	{
		*fp = stdin;
	}
}

/*
 * Function:  main
 * --------------------
 * 
 * The program takes a textfile of user-accounts,
 * append their repectivly UID:s and usernames to a linked list.
 * The program sorts the list and prints the accounts.
 * The textfile can be imported via inputparameter aswell as stdin.
 *
 * Example execution:    ./mpasswdsort infile
 *                         ./mpasswwdsort < infile
 *
 * Example output:
 *                         1171:mr
 *                        12037:set
 *                        12856:axelsson
 *                        12928:dahlin
 *                        16928:gabriel
 *                        17847:fahlgren
 *
 *  argc:        number input-parameters
 *  argv[]:      Char* pointing to a char-array of input-parameters
 *
 *  returns:    EXIT_FAILURE if something goes wrong or a text-row is invalid.
 *                 EXIT_SUCCESS if execution went succesfully.
 */
int main(int argc, char *argv[])
{

	node *list = linkedList_create();
	FILE *fp = NULL;

	bool foundFaultyRow = false;

	if (argc > 2) // Too many input-parameters
	{
		fprintf(stderr, "ERROR: To many parameters\n");
		linkedList_free(list);
		exit(EXIT_FAILURE);
	}

	openFileInput(argc, argv, &fp);

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
				parseLine(bufsize, &foundFaultyRow, line, lineNumber, list);
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

	if (foundFaultyRow)
	{
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}
