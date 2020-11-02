#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "process.h"

struct QueueNode {
    Process *val;
    struct QueueNode *next;
};

typedef struct QueueNode QueueNode;

struct Queue {
    QueueNode *next;
    QueueNode *last;
    int length;
};

typedef struct Queue Queue;

Queue *new_queue(void);

void enqueue(Queue *q, Process *p);

Process *peek(Queue *q);

Process *dequeue(Queue *q);

void print_queue(Queue q);

#endif