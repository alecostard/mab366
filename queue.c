#include "queue.h"

Queue *new_queue(void) {
    Queue *q = malloc(sizeof(Queue));
    q->next = NULL;
    q->last = NULL;
    q->length = 0;
    return q;
}

void enqueue(Queue *q, PCB *p) {
    QueueNode *node = malloc(sizeof(QueueNode));
    node->val = p;
    node->next = NULL;

    if (q->length == 0) {
        q->next = node;
        q->last = node;
    } else {
        q->last->next = node;
        q->last = node;
    }
    q->length++;
}

/* fila não pode estar vazia */
PCB *peek(Queue *q) {
    return q->next->val;
}

/* fila não pode estar vazia */
PCB *dequeue(Queue *q) {
    QueueNode *n = q->next;
    PCB *result = n->val;
    
    q->next = q->next->next;
    q->length--;

    free(n);

    return result;
}
