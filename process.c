#include "process.h"

PCB *new_process(int pid, int required_service) {
    PCB *p = malloc(sizeof(PCB));
    p->pid = pid;
    p->required_service = required_service;
    p->realized_service = 0;
    p->ios = new_io_list();
    return p;
}

IOList *new_io_list() {
    IOList *list = malloc(sizeof(IOList));
    list->head = NULL;
    return list;
}

IOReq *new_io_request(IOType type, int start, int duration) {
    IOReq *req = malloc(sizeof(IOReq));
    req->type = type;
    req->start = start;
    req->duration = duration;
    return req;
}

void add_io(PCB *process, IOType type, int start, int duration) {
    IOList *list = process->ios;
    IONode *node = malloc(sizeof(IONode));
    node->req = new_io_request(type, start, duration);
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        IONode *tracer = list->head;
        while (tracer->next != NULL) {
            tracer = tracer->next;
        }
        tracer->next = node;
    }
}

bool process_finished(PCB *p) {
    return p->realized_service == p->required_service;
}

bool has_ios(PCB *p) {
    return p->ios->head != NULL;
}

/* lista de io não pode estar vazia */
IOReq *peek_io(PCB *p) {
    return p->ios->head->req;
}

IOReq *pop_io(PCB *p) {
    IONode *node = p->ios->head;
    IOReq *req = node->req;

    p->ios->head = p->ios->head->next;
    free(node);

    return req;    
}

char *io_to_s(IOType type) {
    switch (type) {
        case DISK:
            return "disco";
        case TAPE:
            return "fita";
        case PRINTER:
            return "impressora";
        default:
            return "dispositivo desconhecido";
    }
}