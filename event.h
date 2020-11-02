#ifndef EVENT_H
#define EVENT_H

#include <stdlib.h>
#include <stdbool.h>
#include "process.h"

enum EventType {
    QUANTUM_START, IO_END, ARRIVAL, QUANTUM_END, IO_REQUEST, IO_START
};

typedef enum EventType EventType;

struct Event {
    int time;
    EventType type;
    Process *process;
};

typedef struct Event Event;

struct EventListNode {
    Event *event;
    struct EventListNode *next;
};

typedef struct EventListNode EventListNode;

struct EventList {
    EventListNode *head;
};

typedef struct EventList EventList;

Event *new_event(int time, EventType type, Process *process);

void destroy_event(Event *event);

int compare_events(Event *e1, Event *e2);

EventList *new_event_list();

bool empty_event_list(EventList *list);

//void destroy_event_list(EventList *list);

void add_event(EventList *list, int time, EventType type, Process *process);

Event *pop_event(EventList *list);

#endif