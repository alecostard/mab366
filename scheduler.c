#include "scheduler.h"

int run_scheduler(EventList *events, int quantum) {
    Queue *ready[2] = {new_queue(), new_queue()};
    int time = 0;
    bool cpu_idle = true;

    while (!empty_event_list(events)) {
        Event *event = pop_event(events);
        PCB *pcb = event->pcb;
        time = event->time;
        printf("[t = %3d] ", time);

        switch (event->type) {
            case ARRIVAL:
                printf("chegada do processo %d\n", pcb->pid);
                enqueue(ready[HIGH], pcb);
                if (cpu_idle) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                }
                break;

            case QUANTUM_START:
                printf("início de execução do processo %d\n", pcb->pid);
                cpu_idle = false;

                if (has_ios(pcb)) {
                    int time_to_io = peek_io(pcb)->start - pcb->realized_service;
                    if (time_to_io > quantum) {
                        pcb->realized_service += quantum;
                        add_event(events, time + quantum, QUANTUM_END, pcb);
                    } else {
                        pcb->realized_service += time_to_io;
                        add_event(events, time + time_to_io, IO_REQUEST, pcb);
                    }
                } else {
                    int time_to_finish = pcb->required_service - pcb->realized_service;
                    if (time_to_finish >= quantum) {  // > ou >= ?
                        pcb->realized_service += quantum;
                        add_event(events, time + quantum, QUANTUM_END, pcb);
                    } else {
                        pcb->realized_service += time_to_finish;
                        add_event(events, time + time_to_finish, QUANTUM_END, pcb);
                    }
                }
                break;

            case QUANTUM_END:
                printf("fim de execução do processo %d\n", pcb->pid);
                if (!process_finished(pcb)) {
                    enqueue(ready[LOW], pcb);
                }
                
                if (ready[HIGH]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                } else if (ready[LOW]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[LOW]));
                } else {
                    cpu_idle = true;
                }
                break;

            case IO_REQUEST:
                printf("processo %d pediu IO\n", pcb->pid);
                add_event(events, time, IO_START, pcb);

                if (ready[HIGH]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                } else if (ready[LOW]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[LOW]));
                } else {
                    cpu_idle = true;
                }
                break;

            case IO_START:
                printf("IO do processo %d iniciou\n", pcb->pid);
                add_event(events, time + pop_io(pcb)->duration, IO_END, pcb);
                break;

            case IO_END:
                printf("IO do processo %d finalizou\n", pcb->pid);
                enqueue(ready[HIGH], pcb);
                if (cpu_idle) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                }
                break;
        }
    }

    return 0;
}
