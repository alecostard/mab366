#ifndef PROCESS_H
#define PROCESS_H
#include <stdlib.h>
#include <stdbool.h>

enum IOType {
    DISK, TAPE, PRINTER
};

typedef enum IOType IOType;

struct IOReq {
    IOType type;
    int start;
};

typedef struct IOReq IOReq;

struct IONode {
    IOReq *req;
    struct IONode *next;
};

typedef struct IONode IONode;

struct IOList {
    IONode *head;
};

typedef struct IOList IOList;

struct PCB {
    int pid;
    int required_service;
    int realized_service;
    IOList *ios;
};

typedef struct PCB PCB;

PCB *new_process(int pid, int required_service);

IOList *new_io_list();

IOReq *new_io_request(IOType type, int start);

void add_io(PCB *pcb, IOType type, int start);

bool has_ios(PCB *pcb);

bool process_finished(PCB *pcb);

IOReq *peek_io(PCB *pcb);

IOReq *pop_io(PCB *pcb);

IOReq *next_io(PCB *pcb);

int io_duration(IOType type);

char *io_to_s(IOType type);

#endif