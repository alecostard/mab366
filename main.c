#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "process.h"
#include "event.h"

EventList *exercicio1();
EventList *exercicio2();
EventList *exercicio3();

int main(void) {
    int quantum = 4;

    EventList *events = exercicio3();

    Queue *ready = new_queue();
    int time = 0;
    bool cpu_idle = true;

    while (!empty_event_list(events)) {
        Event *event = pop_event(events);
        Process *process = event->process;
        time = event->time;
        printf("[t = %3d] ", time);

        switch (event->type) {
            case ARRIVAL:
                printf("chegada do processo %d\n", process->pid);
                enqueue(ready, process);
                if (cpu_idle) {
                    add_event(events, time, QUANTUM_START, dequeue(ready));
                }
                break;
            case QUANTUM_START:
                printf("início de execução do processo %d\n", process->pid);
                cpu_idle = false;

                if (has_ios(process)) {
                    int time_to_io = peek_io(process)->start - process->realized_service;
                    if (time_to_io > quantum) {
                        process->realized_service += quantum;
                        add_event(events, time + quantum, QUANTUM_END, process);
                    } else {
                        process->realized_service += time_to_io;
                        add_event(events, time + time_to_io, IO_REQUEST, process);
                    }
                } else {
                    int time_to_finish = process->required_service - process->realized_service;
                    if (time_to_finish >= quantum) {
                        process->realized_service += quantum;
                        add_event(events, time + quantum, QUANTUM_END, process);
                    } else {
                        process->realized_service += time_to_finish;
                        add_event(events, time + time_to_finish, QUANTUM_END, process);
                    }
                }
                break;

            case QUANTUM_END:
                printf("fim de execução do processo %d\n", process->pid);
                if (!process_finished(process)) {
                    enqueue(ready, process);
                }
                if (ready->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready));
                } else {
                    cpu_idle = true;
                }
                break;

            case IO_REQUEST:
                printf("processo %d pediu IO\n", process->pid);
                add_event(events, time, IO_START, process);
                if (ready->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready));
                } else {
                    cpu_idle = true;
                }
                break;

            case IO_START:
                printf("IO do processo %d iniciou\n", process->pid);
                add_event(events, time + pop_io(process)->duration, IO_END, process);
                break;

            case IO_END:
                printf("IO do processo %d finalizou\n", process->pid);
                enqueue(ready, process);
                if (cpu_idle) {
                    add_event(events, time, QUANTUM_START, dequeue(ready));
                }
                break;
        }
    }

    return 0;
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
    Process *p1 = new_process(1, 13);
    Process *p2 = new_process(2, 11);
    Process *p3 = new_process(3,  7);
    Process *p4 = new_process(4,  8);
    Process *p5 = new_process(5, 16);

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