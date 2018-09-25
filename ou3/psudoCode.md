# Mish Psudocode (EMIL SÖDERLIND)

* Create PID-Array

* Print prompt

* Import command-row

* Command-row --> Parser --> CommandLine[] + NrOfCommands

* If CommandLine[0] == "Echo"
    * Run internal echo
    * Go back to prompt
* If CommandLine[0] == "cd"
    * Run internal cd
    * Go back to prompt

* else
    * Create pipes (#NrOfCommands-1)
    * Pipe-index = 0 | Keep track of filedesc to close/open etc)
    * For each command in CommandLine[]
        * PID = fork()
        * Check PID (!= -1)
        * Append to PID-Array | PARENT
        * If(PID == 0) | CHILD 
            * If(Pipe-index == 0 && NrOfCommands != 0) | First command and NOT only 1 command
                * Close STDOUT
                * DUP STDOUT+Pipe-index+1 -Write-> STDOUT
                * Close STDOUT+Pipe-index+1
            * else if(Pipe-index == 1 && NrOfCommands != 0) | Last command and NOT only 1 command
                * Close STDIN
                * DUP STOUT+Pipe-index -Read-> STDIN
                * Close STDOUT+Pipe-index
            * else if(NrOfCommands != 0)| Middle commands and NOT only 1 command
                * Close STDOUT
                * DUP STDOUT+Pipe-index+1 -Write-> STDOUT
                * Close STDOUT+Pipe-index+1
                * Close STDIN
                * DUP STOUT+Pipe-index -Read-> STDIN
                * Close STDOUT+Pipe-index
            * If there is redirects ( > < ) etc
                * Override STDOUT/STDIN with redirect
            * Execv (command) | Child will end here
        * Pipe-index ++ | Parent will run this
    * Wait for CHILDS | PARENT