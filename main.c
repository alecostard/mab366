#include <stdio.h>
#include <stdbool.h>
#include "scheduler.h"
#include "process.h"
#include "event.h"

EventList *setup();
EventList *exercicio1();
EventList *exercicio2();
EventList *exercicio3();


int main(void) {
    // run_scheduler(exercicio3(), 4);
    run_scheduler(setup(), 4);
    return 0;
}

EventList *setup() {
    PCB *p1 = new_process(1, 10);
    PCB *p2 = new_process(2, 10);
    PCB *p3 = new_process(3, 30);

    add_io(p1, DISK, 3, 5);
    add_io(p2, TAPE, 1, 4);

    EventList *events = new_event_list();
    add_event(events, 0, ARRIVAL, p1);
    add_event(events, 0, ARRIVAL, p2);
    add_event(events, 0, ARRIVAL, p3);

    return events;    
}

EventList *exercicio1() {
    EventList *events = new_event_list();
    add_event(events, 0, ARRIVAL, new_process(1, 10));
    add_event(events, 0, ARRIVAL, new_process(2,  1));
    add_event(events, 0, ARRIVAL, new_process(3,  2));
    add_event(events, 0, ARRIVAL, new_process(4,  1));
    add_event(events, 0, ARRIVAL, new_process(5,  5));

    return events;
}

EventList *exercicio2() {
    EventList *events = new_event_list();
    add_event(events,  0, ARRIVAL, new_process(1, 13));
    add_event(events,  4, ARRIVAL, new_process(2, 11));
    add_event(events,  5, ARRIVAL, new_process(3,  7));
    add_event(events,  7, ARRIVAL, new_process(4,  8));
    add_event(events, 10, ARRIVAL, new_process(5, 16));

    return events;
}

EventList *exercicio3() {
    PCB *p1 = new_process(1, 13);
    PCB *p2 = new_process(2, 11);
    PCB *p3 = new_process(3,  7);
    PCB *p4 = new_process(4,  8);
    PCB *p5 = new_process(5, 16);

    add_io(p1, 0, 4, 7);
    add_io(p2, 1, 2, 4);
    add_io(p2, 0, 6, 7);
    add_io(p5, 0, 2, 7);
    add_io(p5, 1, 7, 4);

    EventList *events = new_event_list();
    add_event(events, 0,  ARRIVAL, p1);
    add_event(events, 4,  ARRIVAL, p2);
    add_event(events, 5,  ARRIVAL, p3);
    add_event(events, 7,  ARRIVAL, p4);
    add_event(events, 10, ARRIVAL, p5);

    return events;
}