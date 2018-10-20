
// Efter tillåtelse från Anton Eriksson använder jag vederbörandes kö från:
// https://github.com/krokerik/5DV088/blob/master/lab3/queue.c

/*
 * queue.c
 *  Anton Eriksson (tfy12aen@cs.umu.se)
 *  Laboration 5 Systemnara programmering HT15
 *  Queue implementation as a singly linked list
 */

#include "queue.h"

/*
 * Create a Node and return it.
 */
Node *Node_create(void)
{
    Node *n = malloc(sizeof(Node));

    if (!n)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    n->name = NULL;
    n->next = NULL;

    return n;
}

/*
 * Create a Node with field 'name' allocated with 'size'.
 */
Node *Node_init(size_t size)
{
    Node *n = Node_create();
    n->name = malloc(size);

    if (!n->name)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    return n;
}

/*
 * Free Node
 */
void Node_free(Node *n)
{
    free(n->name);
    free(n);
}

/*
 * Create Queue
 */
Queue *Queue_create(void)
{
    Queue *q = malloc(sizeof(Queue));

    if (!q)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    q->front = q->end = NULL;
    q->length = 0;

    return q;
}

/*
 * Return the length of Queue.
 */
int Queue_length(Queue *q)
{
    return q->length;
}

/*
 * Is the Queue empty?
 */
bool Queue_isempty(Queue *q)
{
    return Queue_length(q) == 0;
}

/*
 * Add Node to end of Queue.
 */
void Enqueue(Queue *q, Node *node)
{
    if (q->end == NULL)
    {
        q->front = q->end = node;
    }
    else
    {
        q->end->next = node;
        q->end = q->end->next;
    }

    q->length++;
}

/*
 * Remove the first Node from Queue and return it.
 *
 * The user is responsible to free the Node afterwards.
 */
Node *Dequeue(Queue *q)
{
    if (q->front == NULL)
    {
        return NULL;
    }

    Node *front = q->front;
    q->front = q->front->next;
    q->length--;

    if (Queue_isempty(q))
    {
        q->front = q->end = NULL;
    }

    front->next = NULL;
    return front;
}

/*
 * Free the Queue and all contained Nodes.
 */
void Queue_free(Queue *q){

    while (!Queue_isempty(q))
    {
        Node *n = Dequeue(q);
        Node_free(n);
    }

    free(q);
}