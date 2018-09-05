#include "list.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	printf("Starting mpasswdsort.c\n");

	printf("argc: %d\n", argc);

	FILE *fp;

	// OM vi har input-parameters ta hand om dom
	if (argc == 2)
	{
		// Om filen fuckar --> felmeddelande
		fp = fopen(argv[1], "r");
	
	}
	else if (argc == 1)
	{ // OM inga input-parameters, kolla stdin.

		fp = stdin;

		// Om ingen stdin --> felmeddelande
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
		char ch;
		while ((ch = fgetc(fp)) != EOF)
		{

			if (strcmp(&ch, "\n") == 0)
			{
				printf("---End of line!\n");
			}
			else
			{
				printf("%c", ch);
			}
		}
	}

	fclose(fp);

	node *rootNode = linkedList_create();

	user_info *value = malloc(sizeof(user_info));
	value->uid = 3;
	value->uname = "Emil";

	user_info *valueTwo = malloc(sizeof(user_info));
	valueTwo->uid = 24;
	valueTwo->uname = "Simon";

	linkedList_append(rootNode, value);
	linkedList_append(rootNode, valueTwo);


	linkedList_print(rootNode);

	linkedList_free(rootNode);

	return 0;
}