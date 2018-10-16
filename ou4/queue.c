
/* Lånat implementation från
 * https://gist.github.com/Groxx/310147
 * Gjort små justeringar själv
 */

/*
	Implementation by Groxx, with absolutely no guarantees, so don't complain to me if it breaks stuff.
	Feel free to use it for awesome, as needed.  Apply liberally, and induce vomiting if you ingest large doses.
	Note from #0d15eb: BREAKING CHANGE FROM #1d1057: this is now a generic storage, storing pointers to data.  Manage your memory accordingly.
	Note: now stores the "next" pointer prior to processing, allowing you to process and pop the first item in the list in one pass without losing your place in the queue (and possibly other shenanigans, this one was just handy for my uses so I changed it).
*/

typedef void *queue_data_type;
struct queue_item
{
    queue_data_type contents;
    struct queue_item *next;
};
struct queue_root
{
    struct queue_item *head;
    struct queue_item *tail;
};

void init_queue(struct queue_root *queue)
{
    queue->head = queue->tail = NULL;
}

void push_queue(struct queue_root *queue, int size, queue_data_type contents)
{
    struct queue_item *item = malloc(sizeof(item));
    item->contents = contents;
    item->next = NULL;
    if (queue->head == NULL)
    {
        queue->head = queue->tail = item;
    }
    else
    {
        queue->tail = queue->tail->next = item;
    }
}

queue_data_type pop_queue(struct queue_root *queue)
{
    queue_data_type popped;
    if (queue->head == NULL)
    {
        return NULL; // causes a compile warning.  Just check for ==NULL when popping.
    }
    else
    {
        popped = queue->head->contents;
        struct queue_item *next = queue->head->next;
        free(queue->head);
        queue->head = next;
        if (queue->head == NULL)
            queue->tail = NULL;
    }
    return popped;
}

void process_queue(struct queue_root *queue, void (*func)(queue_data_type))
{
    if (queue == NULL)
        return;
    struct queue_item *current = queue->head;
    while (current != NULL)
    {
        next = current->next
                   func(current->contents);
        current = next;
    }
}