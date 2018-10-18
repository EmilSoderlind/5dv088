
// Efter tillåtelse från Anton Eriksson använder jag vederbörandes kö från:
// https://github.com/krokerik/5DV088/blob/master/lab3/queue.c

/*
 * queue.h
 *  Anton Eriksson (tfy12aen@cs.umu.se)
 *  Laboration 5 Systemnara programmering HT15
 *  Queue implementation as a singly linked list
  */

#ifndef QUEUE_H
#define QUEUE_H

/*! \cond */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/*! \endcond */

typedef struct Node Node;

/*
 * A Node in the Queue
 */
struct Node
{
    /* The data stored */
    char *name;

    /* Next Node in Queue */
    Node *next;
};

/*
 * A Queue of Nodes.
 */
typedef struct Queue
{
    /* The front of the Queue */
    Node *front;

    /* The end of the Queue */
    Node *end;

    /* Number of Nodes in the Queue */
    int length;
} Queue;

/*
 * Create a Node and return it.
 */
Node *Node_create(void);

/*
 * Create a Node with field 'name' allocated with 'size'.
 */
Node *Node_init(size_t size);

/*
 * Free Node
 */
void Node_free(Node *n);

/*
 * Create Queue
 */
Queue *Queue_create(void);

/*
 * Return the length of Queue.
 */
int Queue_length(Queue *q);

/*
 * Is the Queue empty?
 */
bool Queue_isempty(Queue *q);

/*
 * Add Node to end of Queue.
 */
void Enqueue(Queue *q, Node *node);

/*
 * Remove the first Node from Queue and return it.
 *
 * The user is responsible to free the Node afterwards.
 */
Node *Dequeue(Queue *q);

/*
 * Free the Queue and all contained Nodes.
 */
void Queue_free(Queue *q);

#endif // QUEUE_H