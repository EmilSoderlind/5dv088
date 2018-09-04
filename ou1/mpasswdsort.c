#include "linkedList.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  //printf("Hello, World! %x \n",rootNode->next);
  printf("Starting mpasswdsort.c\n");

  node *rootNode = linkedList_create();

  printf("2\n");

  user_info *testUser = malloc(sizeof(user_info));

  testUser->uid = 3;

  testUser->uname = "Hello";

  printf("%s\n", testUser->uname);

  printf("3\n");

  linkedList_append(rootNode,testUser);

  linkedList_print(rootNode);

  linkedList_free(rootNode);


  return 0;
}
