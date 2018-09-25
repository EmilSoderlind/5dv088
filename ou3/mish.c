#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#include "mish.h"

#include "execute.h"
#include "parser.h"

static const unsigned int MAX_COMMANDS = 20;

int internal_echo(int argc, char *argv[]){

    


}

int internal_cd(int argc, char *argv[]){


    if (chdir(dir) != 0){
        fprintf(stderr, "ERROR: cd | Could not change directory");
        return 1;
    }

    return 0;
}

// REMOVE LATER !!
void print_command(command com){
    int i = 0;

    printf("{\n");
    printf("  Argv: [");

    while (*(com.argv + i) != NULL)
    {
        /* Print each argument. */
        printf("\"%s\"", (*(com.argv + i)));
        if (i < com.argc - 1)
            printf(", ");

        i++;
    }
    printf("]\n");

    printf("  Argc: %d\n", com.argc);
    printf("  Infile: %s\n", com.infile);
    printf("  Outfile: %s\n", com.outfile);
    printf("}\n");
}

// Wait for input from user
int prompt(void){

    printf("mish%%: ");

    char promptLine[MAXWORDS];

    if (fgets(promptLine, MAXWORDS, stdin) != NULL)
    {
        // ERROR
        puts(promptLine);
    }

    command comLine[MAX_COMMANDS + 1];

    int numberOfCommands = parse(promptLine, comLine);

    for(int i = 0; i<numberOfCommands; i++){
    
    
        print_command(comLine[i]);
    

    
    }

    
    return 0;
}

int runCommand(command com){

    // Internt kommando, kör själv    



    // Externt kommand:
    // Forka barn + Koppla ihop barn + smara barn PID

}

int main(int argc, char *argv[]) {


    prompt();



    return 0;
}
