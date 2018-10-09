
#include "mish.h"

int PROCESS_PID = 1337;

int internal_echo(int argc, char *argv[]){

    for (int i = 1; i<argc; i++){
        if(argc <= 2){
            fprintf(stdout,"%s", argv[i]);
        }else{
            fprintf(stdout,"%s ", argv[i]);
        }
    }
    
    //fflush(stdout);
    fprintf(stdout, "\n");

    return 0;
}

int internal_cd(char *argv[]){

    char *directory = "";

    if (argv[1] == NULL){
        directory = getenv("HOME");
    }else{
        directory = argv[1];
    }

    if (chdir(directory) < 0){
        fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
        return -1;
    }

    return 0;
}

// Promt prompt, wait for input, parse input and fill CommandLine[] + NrOfCommands
int prompt(command commandArr[], int* NrOfCommands){

    fprintf(stderr, "mish%% ");
    //printf("mish%% ");
    fflush(stderr);

    char promptLine[MAXWORDS];
    promptLine[0] = '\0';

    if (fgets(promptLine, MAXWORDS, stdin) == NULL){
        // ERROR
        puts(promptLine);
        
        exit(0);

        return -1;
    }

    if (strlen(promptLine) == 1){
        return 0;
    }

    *NrOfCommands = parse(promptLine, commandArr);

    return 0;
}

int runCommand(command com, int commandIndex, int nrOfCommands, int pipeArray[][2]){

    //print_command(com);
    int tempPID; // For saving down in parent

    // Fork
    if ((tempPID = fork()) < 0){
        // ERROR
        perror("Error forking!");
        return -1;
    }

    if (tempPID != 0){ // Parent <- Save childrens PID
        printf("PID_CHILDREN_ARRAY[%d] = %d \n", NR_OF_CHILDREN, tempPID);
        PID_CHILDREN_ARRAY[NR_OF_CHILDREN] = tempPID;
        NR_OF_CHILDREN++;
    }else{ // Children <- Set childrens PROCESS_PID
        PROCESS_PID = tempPID;
    }

    if(PROCESS_PID == 0){ // CHILD

        if(nrOfCommands == 1){ // Only 1 command
            
        }else if(commandIndex == 0){ // First command in chain

            dupPipe(pipeArray[0],WRITE_END,STDOUT_FILENO);
            close(pipeArray[0][READ_END]);

            for (int i = 1; i < (nrOfCommands - 1); i++) // Close unused pipes
            {
                close(pipeArray[i][READ_END]);
                close(pipeArray[i][WRITE_END]);
            }
       
        }else if((commandIndex+1) == nrOfCommands){ // Last command in chain

            dupPipe(pipeArray[commandIndex-1], READ_END, STDIN_FILENO);
            close(pipeArray[commandIndex - 1][WRITE_END]);

            // Loope through pipes and close unused
            for (int i = 0; i < (nrOfCommands - 1); i++){

                if (i != (commandIndex - 1)) {
                    close(pipeArray[i][READ_END]);
                    close(pipeArray[i][WRITE_END]);
                }
            }
        }else{ // Ordinary commands in chain

            // Loope through pipes and open/close dup when nessecary
            for (int i = 0; i < (nrOfCommands - 1); i++){

                if (i == (commandIndex - 1)){
                    dupPipe(pipeArray[i], READ_END, STDIN_FILENO);
                    close(pipeArray[i][WRITE_END]);
                }else if (i == (commandIndex)){
                    dupPipe(pipeArray[i], WRITE_END, STDOUT_FILENO);
                    close(pipeArray[i][READ_END]);
                }
                else // Close unused pipes
                {
                    close(pipeArray[i][READ_END]);
                    close(pipeArray[i][WRITE_END]);
                }
            }
        }

        // Skriv ut till fil osv redirect
        // First in chain - in-File redirect
        if(com.infile){
            redirect(com.infile, O_RDONLY, STDIN_FILENO);
        }

        // Last in chain - out-File redirect
        if (com.outfile){
            redirect(com.outfile, ( O_WRONLY | O_EXCL | O_CREAT ) , STDOUT_FILENO);
        }

        if (execvp(com.argv[0], com.argv) < 0){
            fprintf(stderr,"%s: ",com.argv[0]);
            perror(" ");
            
            // It have fucked up --> Close all filedesc
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
    if (prompt(comLine, &NrOfCommands) != 0){
        // ERROR
        fprintf(stderr, "Error in prompt!\n");
        return -1;
    }

    // If prompt got nothin g, return 0 and try again.
    if (NrOfCommands == 0){
        return 0;
    }

    // Check for redirect in middle of chain (non start/end) --> Return 0;
    for (int i = 1; i < NrOfCommands - 1; i++){

        if (comLine[i].infile != NULL){
            fprintf(stderr, "Input from file is only allowed first in command-chain\n");
            return 0;
        }
        if (comLine[i].outfile != NULL)
        {
            fprintf(stderr, "Output to file is only allowed last in command-chain\n");
            return 0;
        }
    }

/*
    if (NrOfCommands > 2){

        // Check for outfile redirect first in chain.
        if (comLine[0].infile){
            printf("First command have infile-Redirect!\n");
            redirect(comLine[0].infile,1,STDIN_FILENO);
            return 0;
        }

        // Check for infile redirect last in chain.
        if (comLine[NrOfCommands].outfile){
            fprintf(stderr, "Last command have outfile-Redirect!\n");
            printf("Last command have outfile-Redirect!\n");
            redirect(comLine[NrOfCommands].outfile, 1, STDOUT_FILENO);
            return 0;
        }
    }*/

    // Internal commands
    if (STRCMP(comLine[0].argv[0], ==, "echo") || STRCMP(comLine[0].argv[0], ==, "cd")){

        if (STRCMP(comLine[0].argv[0], ==, "cd")){
            internal_cd(comLine[0].argv);
        }
        if (STRCMP(comLine[0].argv[0], ==, "echo")){
            internal_echo(comLine[0].argc, comLine[0].argv);
        }
    
    }else{ 
        
        // External commands

        // BUILD INT ARRAY ARRAY
        int pipeArray[NrOfCommands - 1][2];
        int commandIndex = 0;

        // CREATE PIPES, fill pipeArray
        for (int i = 0; i < (NrOfCommands - 1); i++) { 
            pipe(pipeArray[i]);
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
                close(pipeArray[i][READ_END]);
                close(pipeArray[i][WRITE_END]);
            }
        }
    }

    // PARENT WAIT
    if(PROCESS_PID != 0) 
    {
        for (int i = 0; i < (int)NR_OF_CHILDREN; i++){
            int status;

            fflush(stderr);
            waitpid(PID_CHILDREN_ARRAY[i], &status, 0);
            PID_CHILDREN_ARRAY[i] = 0;

        }
    }

    NR_OF_CHILDREN = 0;
    return 0;
}

int loopRunShell(void){

    if (signalHand(SIGINT, killChildren) == SIG_ERR){
        fprintf(stderr, "Couldn't register signal handler\n");
        exit(1);
    }

    while (true)
    {
        if (runShell() != 0)
        {
            fprintf(stderr, "Error in runShell()\n");
            perror("e --> ");
            return -1;
        }
    }

    return 0;
}

void killChildren(int sig)
{

    printf("NR_OF_CHILDREN: %d\n", NR_OF_CHILDREN);
    
    // Kill children
    for (int i = 0; i < (int)NR_OF_CHILDREN; i++)
    {   
        sig++;
        fprintf(stderr, "Killing PID[%d]:%d\n", i, PID_CHILDREN_ARRAY[i]);
        if (kill(PID_CHILDREN_ARRAY[i], SIGKILL) < 0)
        {
            fprintf(stderr, "Error killing child!\n");
        }
    }
    NR_OF_CHILDREN = 0;
}

int main(void) {

    if (loopRunShell() == 0){
        return 0;
    }else{
        return -1;
    }
    return 0;
}
