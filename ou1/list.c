#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Create linkedList. Returns root-node.
node *linkedList_create(){

  node *rootNode = malloc(sizeof(node));

  rootNode->prev = NULL;
  rootNode->next = NULL;
  rootNode->value = NULL;

  return rootNode;
}

// Traverse through list and free one after the other.
void linkedList_free(node *rootNode){

  bool doneTraverse = false;
  node *currentNode = rootNode;

  while(doneTraverse == false){

    if(currentNode->value != NULL){ // if value is NOT freed, free it.

      user_info *currValue = currentNode->value;
      //free(currValue->uname);
      free(currValue);
    }

    if(currentNode->prev != NULL){ // On ordinary nodes (NOT ROOT)
      free(currentNode->prev);
    }

    // Ordinary nodes (Prev pointer != NULL)
    if(currentNode->next == NULL){ // Reached last node
      doneTraverse = true;
      free(currentNode);
    }else{

      currentNode = currentNode->next;
    }
  }


}

void linkedList_append(node *rootNode, user_info *value){

  node *lastNode = NULL;
  node *currentNode = rootNode;
  bool foundLastNode = false;

  while(!foundLastNode){

    if(currentNode->next == NULL){ // Reached last node
      foundLastNode = true;
      lastNode = currentNode;
    }else{
      currentNode = currentNode->next;
    }

  }

  lastNode->next = malloc(sizeof(node));
  ((node*)(lastNode->next))->prev = lastNode; // New node points to previos
  ((node*)(lastNode->next))->value = value; // New node points to its value
  ((node*)(lastNode->next))->next = NULL; // New node next-pointer is NULL

}

void linkedList_print(node *rootNode){

  node *currentNode = rootNode;
  bool foundLastNode = false;
  int index = 0;

  while(!foundLastNode){

    user_info *currentValue = currentNode->value;

    if(currentValue != NULL){ // If not root node
      printf("%d:%s\n", currentValue->uid, currentValue->uname);
    }else{
      /*
      printf("%d\n", index);
      printf("uid: root\n");
      printf("uname: root\n");
      printf("--\n");
      */
    }

    if(currentNode->next == NULL){
      foundLastNode = true;
    }else{
      currentNode = currentNode->next;
      index = index + 1;
    }

  }

}

void linkedList_swapNodes(node *a, node *b)
{

  user_info* tempA_UI = a->value;
  user_info* tempB_UI = b->value;

  a->value = tempB_UI;
  b->value = tempA_UI;

}


void linkedList_sort(node *start)
{

  bool swapped = true;

  node *node1 = start->next;

  if (start->next == NULL) // Empty list
  {
    fprintf(stderr, "ERROR: Empty file\n");
    exit(1);
  }

  do
  {
    swapped = false;
    node1 = start->next;

    while (node1->next != NULL)
    {

      unsigned int uid1 = (unsigned int)(((user_info*)(node1->value))->uid);
      user_info *nextUI = ((user_info *)(node1->next)->value);
      unsigned int uid2 = (unsigned int)(nextUI->uid);
      
      if(uid1 > uid2)
      {
        linkedList_swapNodes(node1,node1->next);
        swapped = true;
      }
      node1 = node1->next;

    }
  } while (swapped);
}