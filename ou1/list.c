#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Function:  node *linkedList_create()
 * --------------------
 *
 *  Creates a linkedlist.
 * 
 *  returns:	Pointer to root-node of linkedlist
 */
node *linkedList_create(void){

  node *rootNode = malloc(sizeof(node));
  if(rootNode == NULL){
    fprintf(stderr, "ERROR: Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  rootNode->prev = NULL;
  rootNode->next = NULL;
  rootNode->value = NULL;

  return rootNode;
}

/*
 * Function:  linkedList_free(node *rootNode)
 * --------------------
 *
 *  Traverse throuh a linkedlist and frees all allocated memory.
 * 
 *  rootNode:	Pointer to root-node of linkedlist to free.
 */
void linkedList_free(node *rootNode)
{

  bool doneTraverse = false;
  node *currentNode = rootNode;

  while(doneTraverse == false){

    if(currentNode->value != NULL){ // if value is NOT freed, free it.

      user_info *currValue = currentNode->value;

      free(currValue->uname);
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

/*
 * Function:  linkedList_append(node *rootNode, void *value)
 * --------------------
 *
 *  Append value (void*) to end of linkedlist.
 * 
 *  rootNode:	  Pointer to root-node of linkedlist to free.
 *  void*:  Pointer to value to append to linkedlist.
 */
void linkedList_append(node *rootNode, void *value)
{

  node *lastNode = NULL;
  node *currentNode = rootNode;
  bool foundLastNode = false;

  while (!foundLastNode)
  {

    if (currentNode->next == NULL)
    { // Reached last node
      foundLastNode = true;
      lastNode = currentNode;
    }
    else
    {
      currentNode = currentNode->next;
    }
  }

  lastNode->next = value;
}
/*
 * Function:  linkedList_addUserInfo(node *rootNode, user_info *value)
 * --------------------
 *
 *  Append user_info to end of linkedlist.
 * 
 *  rootNode:	  Pointer to root-node of linkedlist to free.
 *  user_info:  Pointer to User_info struct to append to linkedlist.
 */
void linkedList_addUserInfo(node *rootNode, user_info *value)
{

  node *lastNode = NULL;
  node *currentNode = rootNode;
  bool foundLastNode = false;

  while (!foundLastNode)
  {

    if (currentNode->next == NULL)
    { // Reached last node
      foundLastNode = true;
      lastNode = currentNode;
    }
    else
    {
      currentNode = currentNode->next;
    }
  }

  lastNode->next = malloc(sizeof(node));
  if(lastNode->next == NULL){
    fprintf(stderr, "ERROR: Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  ((node *)(lastNode->next))->prev = lastNode; // New node points to previos
  ((node *)(lastNode->next))->value = value;   // New node points to its value
  ((node *)(lastNode->next))->next = NULL;     // New node next-pointer is NULL
  }

  /*
 * Function:  linkedList_print(node *rootNode)
 * --------------------
 *
 *  Prints content of linkedlist
 * 
 *  rootNode:	  Pointer to root-node of linkedlist to print.
 */
  void linkedList_print(node * rootNode)
  {

    node *currentNode = rootNode;
    bool foundLastNode = false;
    int index = 0;

    while (!foundLastNode)
    {

      user_info *currentValue = currentNode->value;

      if (currentValue != NULL)
      { // If not root node
        printf("%d:%s\n", currentValue->uid, currentValue->uname);
      }

      if (currentNode->next == NULL)
      {
        foundLastNode = true;
      }
      else
      {
        currentNode = currentNode->next;
        index = index + 1;
      }
    }
  }

  /*
 * Function:  linkedList_print(node *rootNode)
 * --------------------
 *
 *  Switch places of two nodes in the linkedlist
 * 
 *  a:	  Pointer to node to swtich with b
 *  b:	  Pointer to node to swtich with a
 */
  void linkedList_swapNodes(node * a, node * b)
  {

    user_info *tempA_UI = a->value;
    user_info *tempB_UI = b->value;

    a->value = tempB_UI;
    b->value = tempA_UI;
  }

  /*
 * Function:  linkedList_sort(node *start)
 * --------------------
 *
 *  Sort linkedlist based on each values:s user_info->UID
 * 
 *  start:	  Pointer to root-node of linkedlist to print.
 */
  void linkedList_sort(node * start)
  {

    bool swapped = true;

    node *node1 = start->next;

    if (start->next == NULL) // Empty list
    {
      return;
    }

    do
    {

      swapped = false;
      node1 = start->next;

      while (node1->next != NULL)
      {

        unsigned int uid1 = (unsigned int)(((user_info *)(node1->value))->uid);
        user_info *nextUI = ((user_info *)(node1->next)->value);
        unsigned int uid2 = (unsigned int)(nextUI->uid);

        if (uid1 > uid2)
        {
          linkedList_swapNodes(node1, node1->next);
          swapped = true;
        }
        node1 = node1->next;
      }
    } while (swapped);
  }