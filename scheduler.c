#include "scheduler.h"

int run_scheduler(EventList *events, int quantum) {
    Queue *ready[2] = {new_queue(), new_queue()};
    Queue *ioqueue[3] = {new_queue(), new_queue(), new_queue()};
    bool io_idle[3] = {true, true, true};
    bool cpu_idle = true;

    while (!empty_event_list(events)) {
        Event *event = pop_event(events);
        PCB *pcb = event->pcb;
        IOType iotype;
        int time = event->time;
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
                    if (time_to_finish > quantum) {
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
                iotype = peek_io(pcb)->type;
                printf("processo %d pediu IO do tipo %s\n", pcb->pid, io_to_s(iotype));

                if (io_idle[iotype]) {
                    add_event(events, time, IO_START, pcb);
                } else {
                    enqueue(ioqueue[iotype], pcb);
                }

                if (ready[HIGH]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                } else if (ready[LOW]->length > 0) {
                    add_event(events, time, QUANTUM_START, dequeue(ready[LOW]));
                } else {
                    cpu_idle = true;
                }
                break;

            case IO_START:
                iotype = peek_io(pcb)->type;
                printf("processo %d iniciou IO do tipo %s\n", pcb->pid, io_to_s(iotype));
                io_idle[iotype] = false;
                add_event(events, time + io_duration(iotype), IO_END, pcb);
                break;

            case IO_END:
                iotype = peek_io(pcb)->type;
                printf("processo %d finalizou IO do tipo %s\n", pcb->pid, io_to_s(iotype));
                
                if (iotype == DISK) {
                    enqueue(ready[LOW], pcb);
                } else {
                    enqueue(ready[HIGH], pcb);
                }

                if (ioqueue[iotype]->length > 0) {
                    add_event(events, time, IO_START, dequeue(ioqueue[iotype]));
                } else {
                    io_idle[iotype] = true;
                }

                pop_io(pcb);

                if (cpu_idle) {
                    if (ready[HIGH]->length > 0) {
                        add_event(events, time, QUANTUM_START, dequeue(ready[HIGH]));
                    } else {
                        add_event(events, time, QUANTUM_START, dequeue(ready[LOW]));
                    }
                }
                break;
        }
    }

    return 0;
}
