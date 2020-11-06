#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "process.h"

struct QueueNode {
    PCB *val;
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

void enqueue(Queue *q, PCB *p);

PCB *peek(Queue *q);

PCB *dequeue(Queue *q);

void print_queue(Queue q);

#endif