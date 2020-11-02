#include <stdio.h>
#include <assert.h>
#include "process.h"
#include "queue.h"
#include "event.h"

void process_tests() {
    Process *p = new_process(1, 10);

    assert(1 == p->pid);
    assert(!process_finished(p));
    assert(!has_ios(p));

    add_io(p, TAPE, 1, 1);
    add_io(p, DISK, 2, 1);
    
    assert(TAPE == p->ios->head->req->type);
    assert(DISK == p->ios->head->next->req->type);
    assert(has_ios(p));

}

void queue_tests() {
    Queue *q = new_queue();
    Process *p1 = new_process(1, 1);
    Process *p2 = new_process(2, 1);
    Process *p3 = new_process(3, 1);

    assert(0 == q->length);

    enqueue(q, p1);
    assert(1 == q->length);
    assert(1 == peek(q)->pid);

    enqueue(q, p2);
    assert(2 == q->length);

    assert(1 == dequeue(q)->pid);
    assert(1 == q->length);

    enqueue(q, p3);
    assert(2 == q->length);

    assert(2 == dequeue(q)->pid);
    assert(3 == dequeue(q)->pid);
    assert(0 == q->length);
}

void event_tests() {
    EventList *l = new_event_list();

    assert(empty_event_list(l));

    add_event(l, 1, ARRIVAL, new_process(1, 1));
    add_event(l, 2, ARRIVAL, new_process(2, 2));
    add_event(l, 2, ARRIVAL, new_process(3, 3));

    assert(!empty_event_list(l));   
    assert(1 == l->head->event->process->pid);
    assert(2 == l->head->next->event->process->pid);
    assert(3 == l->head->next->next->event->process->pid);

    assert(1 == pop_event(l)->process->pid);
    assert(2 == pop_event(l)->process->pid);
    assert(3 == pop_event(l)->process->pid);
    assert(empty_event_list(l));
}


int main(void) {
    process_tests();
    // queue_tests();
    // event_tests();
    return 0;
}