#include "event.h"

Event *new_event(int time, EventType type, PCB *pcb) {
    Event *event = malloc(sizeof(Event));
    event->time = time;
    event->type = type;
    event->pcb = pcb;
    return event;
}

void destroy_event(Event *event) {
    free(event);
}

int compare_events(Event *e1, Event *e2) {
    if (e1->time < e2->time) {
        return -1;
    } else if (e1->time > e2->time) {
        return 1;
    } else if (e1->type < e2->type) {
        return -1;
    } else if (e1->type > e2->type) {
        return 1;
    } else {
        return 0;
    }
}

EventList *new_event_list() {
    EventList *list = malloc(sizeof(EventList));
    list->head = NULL;
    return list;
}

bool empty_event_list(EventList *list) {
    return list->head == NULL;
}

//void destroy_event_list(EventList *event_list);

void add_event(EventList *list, int time, EventType type, PCB *pcb) {
    Event *event = new_event(time, type, pcb);
    EventListNode *node = malloc(sizeof(EventListNode));
    node->event = event;

    if (list->head == NULL || compare_events(event, list->head->event) == -1) {
        node->next = list->head;
        list->head = node;
    } else {
        EventListNode *tracer = list->head;
        while (tracer->next != NULL && compare_events(event, tracer->next->event) != -1) {
            tracer = tracer->next;
        }
        
        node->next = tracer->next;
        tracer->next = node;
    }
} 

/* lista não pode estar vazia */
Event *pop_event(EventList *list) {
    EventListNode *old_head = list->head;
    Event *result = old_head->event;
    list->head = list->head->next;
    free(old_head);
    return result;
}