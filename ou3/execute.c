#include <stdio.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close

#include "execute.h"

/* Duplicate a pipe to a standard I/O file descriptor
 * Arguments:	pip	the pipe
 *		end	tells which end of the pipe shold be dup'ed; it can be
 *			one of READ_END or WRITE_END
 *		destfd	the standard I/O file descriptor to be replaced
 * Returns:	-1 on error, else destfd
 */

int dupPipe(int pip[2], int end, int destfd){

    if(close(destfd) == -1){
        return -1;
    }

    int duplicatedFileDesc = dup(pip[end]);

    if(duplicatedFileDesc == -1){
        return duplicatedFileDesc;
    }

    if(close(pip[end]) == -1){
        return -1;
    }
   
    return duplicatedFileDesc;
}

/* Redirect a standard I/O file descriptor to a file
 * Arguments:	filename	the file to/from which the standard I/O file
 * 				descriptor should be redirected
 * 		flags	indicates whether the file should be opened for reading
 * 			or writing
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */
int redirect(char *filename, int flags, int destfd){

    int fileDesc;

    // IF WRITE
    if(flags ){ 

        if ((fileDesc = open(filename, flags)) == -1){
            fprintf(stdout, "Could not open file\n"); // "Hello world" on stdout (using fprintf)
            return -1; // Error opening file
        }

        if(close(destfd) == -1){
            fprintf(stdout, "Could not close fileDescriptor %d\n", destfd); // "Hello world" on stdout (using fprintf)
            return -1;
        }

        int shouldBeDestfd = dup(fileDesc);

        if(shouldBeDestfd != destfd){
            fprintf(stdout, "Could not dup-fileDescriptor %d\n", destfd); // "Hello world" on stdout (using fprintf)

            return -1;
        }

    }
    
    return destfd;
}
