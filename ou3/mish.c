
#include "mish.h"

static unsigned int NR_OF_CHILDREN = 0;
static int PID_CHILDREN_ARRAY[MAXCOMMANDS];



int internal_echo(int argc, char *argv[]){

    for (int i = 1; i<argc; i++){
        if(argc <= 2){
            printf("%s", argv[i]);
        }else{
            printf("%s ", argv[i]);
        }
    }

    fflush(stdout);
    fputs("\n", stderr);

    return 0;
}

int internal_cd(int argc, char *argv[]){

    char *dir = argv[1] == NULL ? getenv("HOME") : argv[1];

    char *directory = "";

    if (argv[1] == NULL){
        directory = getenv("HOME");
    }else{
        directory = argv[1];
    }

    if (chdir(directory) != 0){
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

    if (fgets(promptLine, MAXWORDS, stdin) == NULL)
    {
        // ERROR
        printf("ERROR! ");
        puts(promptLine);
        return 1;
    }

    *NrOfCommands = parse(promptLine, commandArr);


    return 0;
}

int runCommand(command com, int pipeIndex, int nrOfCommands, int pipeArray[][2]){

    // Internt kommando, kör själv


    print_command(com);



    // Externt kommand:
    // Forka barn + Koppla ihop barn + smara barn PID
    
    return 0;
}

int main(int argc, char *argv[]) {

    command comLine[MAX_COMMANDS + 1];
    int NrOfCommands = 0;

    // Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
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
        if (STRCMP(comLine[0].argv[0], ==, "echo")){
            internal_echo(comLine[0].argc, comLine[0].argv);
        }
    }else{ // External commands

        // BUILD INT ARRAY ARRAY
        int pipeArray[NrOfCommands-1][2];
        int pipeIndex = 0;

        for (int i = 0; i < (NrOfCommands - 1); i++){ // Create pipes, fill pipeArray
            pipe(pipeArray[i]);
        }

        for(int i = 0; i<NrOfCommands; i++){
            runCommand(comLine[i],pipeIndex,NrOfCommands,pipeArray);
        }

    }

    
    return 0;
}
