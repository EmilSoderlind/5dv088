#include "linkedList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int returnNumber(int k){
  return k;
}

// Create linkedList. Returns root-node.
node *linkedList_create(){
  printf("linkedList_create\n");

  node *rootNode = malloc(sizeof(node));

  rootNode->prev = NULL;
  rootNode->next = NULL;
  rootNode->value = NULL;

  printf("linkedList_create - DONE\n");
  return rootNode;
}

// Traverse through list and free one after the other.
void linkedList_free(node *rootNode){

  bool doneTraverse = false;
  node *currentNode = rootNode;

  while(doneTraverse == false){

    if(currentNode->value != NULL){ // if value is NOT freed, free it.

      user_info *currValue = currentNode->value;
      free(currValue->uname);
      free(currValue);
    }

    // Ordinary nodes (Prev pointer != NULL)
    if(currentNode->next == NULL){ // Reached last node
      doneTraverse = true;
      free(currentNode);
    }else{
      node *currentNode = currentNode->next;
    }

    if(currentNode->prev != NULL){ // On ordinary nodes (NOT ROOT)
      free(currentNode->prev);
    }

  }

}

void linkedList_append(node *rootNode, user_info *value){
  printf("linkedList_append");


  node *lastNode = NULL;
  node *currentNode = rootNode;
  bool foundLastNode = false;

  while(!foundLastNode){

    if(currentNode->next == NULL){ // Reached last node
      foundLastNode = true;
    }else{
      currentNode = currentNode->next;
    }

  }

  lastNode->next = malloc(sizeof(node));
  (lastNode->next)->prev = lastNode; // New node points to previos
  (lastNode->next)->value = value; // New node points to its value

  printf("linkedList_append - DONE");
}

void linkedList_print(node *rootNode){
  printf("linkedList_print\n");

  node *currentNode = rootNode;
  bool foundLastNode = false;
  int index = 0;

  while(!foundLastNode){

    user_info *currentValue = currentNode->value;

    if(currentValue != NULL){ // If not root node
      printf("%d", index);
      printf("uid: %x\n",currentValue->uid);
      printf("uname: %s\n",currentValue->uname);
      printf("--");
    }else{
      printf("ROOT-NOOODE!\n");
    }

    if(currentNode->next == NULL){
      foundLastNode = true;
    }else{
      currentNode = currentNode->next;
      index = index + 1;
    }

  }

  printf("linkedList_print - DONE\n");
}
