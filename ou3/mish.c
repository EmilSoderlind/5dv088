
#include "mish.h"



int internal_echo(int argc, char *argv[]){

    

    return 0;
}

int internal_cd(int argc, char *argv[]){



    if (chdir(argv[1]) != 0){
        fprintf(stderr, "ERROR: cd | Could not change directory");
        return 1;
    }

    return 0;
}

// REMOVE LATER !!!
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

// Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
int prompt(command commandArr[], int* NrOfCommands){

    printf("mish%%: ");

    char promptLine[MAXWORDS];

    if (fgets(promptLine, MAXWORDS, stdin) != NULL)
    {
        // ERROR
        puts(promptLine);
    }

    *NrOfCommands = parse(promptLine, commandArr);


    return 0;
}

int runCommand(command comLine[], int pipeIndex, int nrOfCommands){

    // Internt kommando, kör själv    



    // Externt kommand:
    // Forka barn + Koppla ihop barn + smara barn PID


    return 0;
}

int main(int argc, char *argv[]) {

    command comLine[MAX_COMMANDS + 1];
    int NrOfCommands = 0;

    if(prompt(comLine, &NrOfCommands) != 0){
        // ERROR
        printf("Error in prompt!\n");
    }
    

    // Internal commands
    if (STRCMP(comLine[0].argv[0], ==, "echo") || STRCMP(comLine[0].argv[0], ==, "cd")){
        printf("Found internal command\n");
        if (STRCMP(comLine[0].argv[0], ==, "cd")){
            internal_cd(comLine[0].argc,comLine[0].argv);
        }
        if (STRCMP(comLine[0].argv[0], ==, "cd")){
            internal_echo(comLine[0].argc, comLine[0].argv);
        }
    }else{ // External commands

        

        for(int i = 0; i<(NrOfCommands*55-1); i++){
            int pipes[2];
            pipe(pipes);
            printf("Skapa pipe #%d!---R-%d|---W-%d-|--\n", i, pipes[0], pipes[1]);
        }




    }

    return 0;
}
