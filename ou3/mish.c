
#include "mish.h"

static unsigned int NR_OF_CHILDREN = 0;
static int PID_CHILDREN_ARRAY[MAXCOMMANDS];
static int PROCESS_PID = 1337;


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

    fprintf(stderr, "mish%% ");
    fflush(stderr);


    char promptLine[MAXWORDS];

    if (fgets(promptLine, MAXWORDS, stdin) == NULL)
    {
        // ERROR
        printf("ERROR! ");
        puts(promptLine);
        return 1;
    }

    *NrOfCommands = parse(promptLine, commandArr);

    for(int k = 0; k<*NrOfCommands;k++){
        print_command(commandArr[k]);
    }

    return 0;
}

int runCommand(command com, int commandIndex, int nrOfCommands, int pipeArray[][2]){

    printf("in runCommand\n");
    print_command(com);

    int tempPID; // For saving down in parent

    printf("Fork!\n");

    // Fork
    if ((tempPID = fork()) < 0){
        // ERROR
        printf("Error forking!");
        return 1;
    }

    if (tempPID != 0){ // Parent <- Save childrens PID
        PID_CHILDREN_ARRAY[NR_OF_CHILDREN] = tempPID;
        NR_OF_CHILDREN++;
        printf("->NR_OF_CHILDREN: %d\n", NR_OF_CHILDREN);
    }else{ // Children <- Set childrens PROCESS_PID
        PROCESS_PID = tempPID;
    }

    if(PROCESS_PID == 0){ // CHILD

        printf("Command: %d/%d\n", commandIndex, nrOfCommands);
        printf("com.argv[0]-> (%s)\n", com.argv[0]);

        // Kasta runt pipor!

        if(nrOfCommands == 1){ // Only 1 command

            printf("Only 1 command!\n");
            

        }else if(commandIndex == 0){ // First command in chain
            printf("First in chain!\n");

            dupPipe(pipeArray[0],WRITE_END,STDOUT_FILENO);
            printf("First through pipe!!\n");

        }else if((commandIndex+1) == nrOfCommands){ // Last command in chain
            printf("Last in chain!\n");

            dupPipe(pipeArray[commandIndex - 1], READ_END, STDIN_FILENO);
            printf("Last with no pipe!!\n");

        }else{ // Ordinary commands in chain
            printf("In chain!\n");

            dupPipe(pipeArray[commandIndex - 1], READ_END, STDIN_FILENO);
            dupPipe(pipeArray[commandIndex], WRITE_END, STDOUT_FILENO);
        }

        // Skriv ut till fil osv redirect

        
        // Excecv!
        printf("Go execvp!\n");
        if (execvp(com.argv[0], com.argv) == -1)
        {
            printf("Could not execute program. (%s) Try again.\n", com.argv[0]);
            exit(-1); // Kill child
        }

    }

    return 0;
}

int runShell(int argc, char *argv[]){

    command comLine[MAX_COMMANDS + 1];
    int NrOfCommands = 0;

    // Get parents pid
    PROCESS_PID = getpid();

    // Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
    if (prompt(comLine, &NrOfCommands) != 0)
    {
        // ERROR
        printf("Error in prompt!\n");
        return -1;
    }

    // Internal commands
    if (STRCMP(comLine[0].argv[0], ==, "echo") || STRCMP(comLine[0].argv[0], ==, "cd"))
    {
        printf("Found internal command\n");
        if (STRCMP(comLine[0].argv[0], ==, "cd"))
        {
            internal_cd(comLine[0].argc, comLine[0].argv);
        }
        if (STRCMP(comLine[0].argv[0], ==, "echo"))
        {
            internal_echo(comLine[0].argc, comLine[0].argv);
        }
    }
    else
    { // External commands

        // BUILD INT ARRAY ARRAY
        int pipeArray[NrOfCommands - 1][2];
        int commandIndex = 0;

        for (int i = 0; i < (NrOfCommands - 1); i++)
        { // Create pipes, fill pipeArray
            pipe(pipeArray[i]);
        }

        for (int i = 0; i < NrOfCommands; i++)
        {
            if(runCommand(comLine[i], commandIndex, NrOfCommands, pipeArray) != 0){
                // ERROR probly could not find command
                return -1;
            }

            // REMOVE LATER JUST DEBUG. Children forks.. -.-
            if (PROCESS_PID == 0){
                break;
            }

            commandIndex++; // Keep track of which command in line to tell child, that is relevant for them
        }
    }

    if (PROCESS_PID != 0)
    { // PARENT WAIT

        for (int i = 0; i < NR_OF_CHILDREN+1; i++)
        {
            int status;
            
            do{
                waitpid(PID_CHILDREN_ARRAY[i], &status, 0);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

            NR_OF_CHILDREN--;
            printf("Parent says: Child exited with status %d \n", status);
            printf("->NR_OF_CHILDREN: %d\n", NR_OF_CHILDREN);
            //printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
            //printf("WIFEXITED: %d\n", WIFEXITED(status));
            //printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
            //printf("WIFSTOPPED: %d\n", WIFSTOPPED(status));
            printf("--\n");
        }
    }else{
        // REMOVE LATER JUST DEBUG
        printf("PID(%d) DONE\n", PROCESS_PID);
        printf("--\n");
    }

    return 0;
}


int main(int argc, char *argv[]) {


    while(1){
        if(runShell(argc,argv) != 0){
            printf("ERROR!\n");
            break;
        }
    }

    fflush(stderr);

    return 0;
}
