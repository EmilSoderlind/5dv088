
#include "mish.h"

static unsigned int NR_OF_CHILDREN = 0;
static int PID_CHILDREN_ARRAY[MAXCOMMANDS];
static int PROCESS_PID = 1337;


int internal_echo(int argc, char *argv[]){

    for (int i = 1; i<argc; i++){
        if(argc <= 2){
            fprintf(stderr,"%s", argv[i]);
        }else{
            fprintf(stderr,"%s ", argv[i]);
        }
    }

    fflush(stdout);
    fputs("\n", stderr);

    return 0;
}

int internal_cd(char *argv[]){

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

    fprintf(stderr, "{\n");
    fprintf(stderr, "  Argv: [");

    while (*(com.argv + i) != NULL)
    {
        /* Print each argument. */
        fprintf(stderr, "\"%s\"", (*(com.argv + i)));
        if (i < com.argc - 1)
            fprintf(stderr, ", ");

        i++;
    }
    fprintf(stderr, "]\n");

    fprintf(stderr, "  Argc: %d\n", com.argc);
    fprintf(stderr, "  Infile: %s\n", com.infile);
    fprintf(stderr, "  Outfile: %s\n", com.outfile);
    fprintf(stderr,"}\n");
}

// Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
int prompt(command commandArr[], int* NrOfCommands){

    fflush(stdout);
    printf("mish%% ");
    fflush(stderr);

    char promptLine[MAXWORDS];
    promptLine[0] = '\0';

    if (fgets(promptLine, MAXWORDS, stdin) == NULL){
        // ERROR
        fprintf(stderr, "ERROR!");
        puts(promptLine);
        return -1;
    }

    fprintf(stderr,"strlen(promptLine): %lu\n", strlen(promptLine));

    if (strlen(promptLine) == 1)
    {
        // NO INPUT
        return -1;
    }

    *NrOfCommands = parse(promptLine, commandArr);

    for(int k = 0; k<*NrOfCommands;k++){
        fprintf(stderr, "Straight out of parse\n");
        print_command(commandArr[k]);
    }

    return 0;
}

int runCommand(command com, int commandIndex, int nrOfCommands, int pipeArray[][2]){

    //print_command(com);

    int tempPID; // For saving down in parent

    // Fork
    if ((tempPID = fork()) < 0){
        // ERROR
        perror("Error forking!");
        return 1;
    }

    if (tempPID != 0){ // Parent <- Save childrens PID
        PID_CHILDREN_ARRAY[NR_OF_CHILDREN] = tempPID;
        NR_OF_CHILDREN++;
        fprintf(stderr, "Fork(%d)->NR_OF_CHILDREN: %d\n", tempPID, NR_OF_CHILDREN);
    }else{ // Children <- Set childrens PROCESS_PID
        PROCESS_PID = tempPID;
    }

    if(PROCESS_PID == 0){ // CHILD

        //printf("Command: %d/%d\n", commandIndex, nrOfCommands);
        fprintf(stderr, "command-> (%s)\n", com.argv[0]);

        if(nrOfCommands == 1){ // Only 1 command
            fprintf(stderr, "Only 1 command!\n");
        }else if(commandIndex == 0){ // First command in chain
            fprintf(stderr, "comInxex: %d/%d | First in chain!\n", commandIndex, nrOfCommands);

            dupPipe(pipeArray[0],WRITE_END,STDOUT_FILENO,com);
            close(pipeArray[0][READ_END]);
            fprintf(stderr, "%s closing %d\n", com.argv[0], pipeArray[0][READ_END]);

            for (int i = 1; i < (nrOfCommands - 1); i++) // Close unused pipes
            {
                fprintf(stderr, "%s closing pipe:%d:%d\n", com.argv[0], pipeArray[i][READ_END], pipeArray[i][WRITE_END]);
                close(pipeArray[i][READ_END]);
                close(pipeArray[i][WRITE_END]);
            }
       
        }else if((commandIndex+1) == nrOfCommands){ // Last command in chain
            fprintf(stderr, "comInxex: %d/%d | Last in chain!\n", commandIndex, nrOfCommands);

            dupPipe(pipeArray[commandIndex-1], READ_END, STDIN_FILENO, com);
            close(pipeArray[commandIndex - 1][WRITE_END]);
            fprintf(stderr, "%s closing %d\n", com.argv[0], pipeArray[commandIndex - 1][WRITE_END]);

            // Loope through pipes and close unused
            for (int i = 0; i < (nrOfCommands - 1); i++){

                // Close unused pipes
                if (i != (commandIndex - 1)) {
                    close(pipeArray[i][READ_END]);
                    close(pipeArray[i][WRITE_END]);
                    fprintf(stderr, "%s closing pipe:%d:%d\n", com.argv[0], pipeArray[i][READ_END], pipeArray[i][WRITE_END]);
                }
            }
        }
        else
        { // Ordinary commands in chain
            fprintf(stderr, "comInxex: %d/%d | Middle of chain!\n", commandIndex, nrOfCommands);

            // Loope through pipes and open/close dup when nessecary
            for (int i = 0; i < (nrOfCommands - 1); i++){

                if (i == (commandIndex - 1)){
                    dupPipe(pipeArray[i], READ_END, STDIN_FILENO,com);
                    close(pipeArray[i][WRITE_END]);
                    fprintf(stderr, "%s closing %d\n", com.argv[0], pipeArray[i][WRITE_END]);
                }else if (i == (commandIndex)){
                    dupPipe(pipeArray[i], WRITE_END, STDOUT_FILENO,com);
                    close(pipeArray[i][READ_END]);
                    fprintf(stderr, "%s closing %d\n", com.argv[0], pipeArray[i][READ_END]);
                }
                else // Close unused pipes
                {
                    close(pipeArray[i][READ_END]);
                    close(pipeArray[i][WRITE_END]);
                    fprintf(stderr, "%s closing pipe:%d:%d\n", com.argv[0], pipeArray[i][READ_END], pipeArray[i][WRITE_END]);
                }
            }
        }

        // Skriv ut till fil osv redirect
        
        // Excecv!
        dprintf(STDERR_FILENO, "Excecv INCOMING! %s\n", com.argv[0]);

        if (execvp(com.argv[0], com.argv) == -1){
            perror("execvp-ERROR: ");
            fprintf(stderr, "execvp() error | Could not execute program. (%s) Try again.\n", com.argv[0]);
            
            // Close all filedesc
            for(int i = 0; i<nrOfCommands-1;i++){
                close(pipeArray[i][READ_END]);
                close(pipeArray[i][WRITE_END]);
                close(STDIN_FILENO);
                close(STDERR_FILENO);
                close(STDOUT_FILENO);
            }
            
            exit(-1); // Kill child
        }
    }
    return 0;
}

int runShell(void){

    command comLine[MAX_COMMANDS + 1];
    int NrOfCommands = 0;

    // Get parents pid
    PROCESS_PID = getpid();

    // Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
    if (prompt(comLine, &NrOfCommands) != 0)
    {
        // ERROR
        fprintf(stderr, "Error in prompt!\n");
        return -1;
    }

    // Internal commands
    if (STRCMP(comLine[0].argv[0], ==, "echo") || STRCMP(comLine[0].argv[0], ==, "cd"))
    {
        fprintf(stderr, "Found internal command\n");
        if (STRCMP(comLine[0].argv[0], ==, "cd"))
        {
            internal_cd(comLine[0].argv);
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

        for (int i = 0; i < (NrOfCommands - 1); i++) // CREATE PIPES, fill pipeArray
        { 
            pipe(pipeArray[i]);
            fprintf(stderr, "Creating pipe: %d:%d\n", pipeArray[i][0], pipeArray[i][1]);
        }

        for (int i = 0; i < NrOfCommands; i++){
            if(runCommand(comLine[i], commandIndex, NrOfCommands, pipeArray) != 0){
                // ERROR probly could not find command
                fprintf(stderr, "ERROR in runCommand()\n");
                return -1;
            }

            // REMOVE LATER JUST DEBUG. Children forks.. -.-
            if (PROCESS_PID == 0){
                break;
            }
            commandIndex++; // Keep track of which command in line to tell child, that is relevant for them
        }

        if(PROCESS_PID != 0){ // CLOSE PIPES IN PARENT
            for (int i = 0; i < (NrOfCommands - 1); i++) // Close unused pipes
            {
                fprintf(stderr, "Parent closing pipe:%d:%d\n", pipeArray[i][READ_END], pipeArray[i][WRITE_END]);

                close(pipeArray[i][READ_END]);
                close(pipeArray[i][WRITE_END]);
            }
        }
    }

    if (PROCESS_PID != 0) // PARENT WAIT
    {
        fprintf(stderr, "Parent starts to wait for (%d childs):\n", NR_OF_CHILDREN);
        fprintf(stderr, "--\n");
        fprintf(stderr, "PID_arr: \n");
        for (int h = 0; h < (int)NR_OF_CHILDREN; h++)
        {
            fprintf(stderr, "%d | %d\n", h, PID_CHILDREN_ARRAY[h]);
        }
        fprintf(stderr, "--\n");

        for (int i = 0; i < (int)NR_OF_CHILDREN; i++)
        {
            int status;

            fprintf(stderr, "wait for PID:%d\n", PID_CHILDREN_ARRAY[i]);
            fflush(stderr);
            waitpid(PID_CHILDREN_ARRAY[i], &status, 0);

            fprintf(stderr, "(%d) is dead ->NR_OF_CHILDREN: %d\n", PID_CHILDREN_ARRAY[i], NR_OF_CHILDREN);
            fprintf(stderr, "Parent says: Child exited with status %d \n", status);
            
            //printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
            //printf("WIFEXITED: %d\n", WIFEXITED(status));
            //printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
            //printf("WIFSTOPPED: %d\n", WIFSTOPPED(status));
            PID_CHILDREN_ARRAY[i] = 0;

            fprintf(stderr, "--\n");
            fprintf(stderr, "PID_arr: \n");
            for (int h = 0; h < (int)NR_OF_CHILDREN; h++)
            {
                fprintf(stderr, "%d | %d\n", h, PID_CHILDREN_ARRAY[h]);
            }
            fprintf(stderr, "--\n");
        }
    }else{
        // REMOVE LATER JUST DEBUG
        fprintf(stderr, "PID(%d) DONE\n", PROCESS_PID);
        fprintf(stderr, "--\n");
    }

    fprintf(stderr, "Parent done with runShell()\n");
    NR_OF_CHILDREN = 0;
    return 0;
}


int main(void) {

    bool loopCond = true;

    while(loopCond){
        fprintf(stderr, "Running MISH!\n");

        if (runShell() != 0)
        {
            fprintf(stderr, "Error in runShell()\n");
            //break;
        }
    }

    return 0;
}
