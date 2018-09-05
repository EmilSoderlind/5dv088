#include "list.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

void fileDemo(int argc, char *argv[]){

  bool parsedInput = false;

  while(!parsedInput){

    FILE *fp;
    char ch;

        fp=fopen(argv[1],"r");

        if(fp==NULL){
            printf("Some problem in opening the file\n");

            // ERROR MESSAGE
            exit(0);
        }else{
            while((ch=fgetc(fp))!=EOF){
                printf("%c",ch);
            }
            break; // TO BE REMOVED
        }

        fclose(fp);

      }

}

int main(int argc, char *argv[]){
  printf("Starting mpasswdsort.c\n");

  printf("argc: %d\n", argc);


// OM vi har input-parameters ta hand om dom
  if(argc == 2){

    // Om filen fuckar --> felmeddelande


  }else if(argc == 1){ // OM inga input-parameters, kolla stdin.

    // Om ingen stdin --> felmeddelande


  }else if(argc > 2){ // OM vi har för många input-parameters --> felmeddelande
    printf("ERROR: To many parameters\n"); //TODO Fixa stderr senare

  }



fileDemo(argc, argv);

  node *rootNode = linkedList_create();


    user_info *value = malloc(sizeof(user_info));
    value->uid = 3;
    value->uname = "Emil";

    user_info *valueTwo = malloc(sizeof(user_info));
    valueTwo->uid = 24;
    valueTwo->uname = "Simon";


  linkedList_append(rootNode, value);
  linkedList_append(rootNode, valueTwo);


/*
  char *text = "Hello";
  unsigned int i = 5;
  user_info* valueThree = user_info_create(i,text);

  linkedList_append(rootNode, valueThree);
*/

  linkedList_print(rootNode);

  linkedList_free(rootNode);

  return 0;
}




user_info* user_info_create(int i, char *name){
  user_info *ui = malloc(sizeof(user_info));
  ui->uid = (unsigned int) i;
  ui->uname = name;

return ui;
}
