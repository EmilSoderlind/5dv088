#include "linkedList.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
  //printf("Hello, World! %x \n",rootNode->next);
  printf("Starting mpasswdsort.c\n");

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
