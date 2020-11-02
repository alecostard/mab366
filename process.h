#ifndef PROCESS_H
#define PROCESS_H
#include <stdlib.h>
#include <stdbool.h>

enum IOType {
    PRINTER, DISK, TAPE
};

typedef enum IOType IOType;

struct IOReq {
    IOType type;
    int start;
    int duration;
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

struct Process {
    int pid;
    int required_service;
    int realized_service;
    IOList *ios;
};

typedef struct Process Process;

Process *new_process(int pid, int required_service);

IOList *new_io_list();

IOReq *new_io_request(IOType type, int start, int duration);

void add_io(Process *process, IOType type, int start, int duration);

bool has_ios(Process *process);

bool process_finished(Process *p);

IOReq *peek_io(Process *p);

IOReq *pop_io(Process *p);

#endif