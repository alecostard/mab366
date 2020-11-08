#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "scheduler.h"
#include "process.h"
#include "event.h"

EventList *load_events(char *filename);
Event *parse_line(char *line);
IOType io_from_s(char *str);

int main(int argc, char *argv[]) {
    EventList *events = load_events(argv[1]);
    int quantum = atoi(argv[2]);
    run_scheduler(events, quantum);
    return 0;
}

EventList *load_events(char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    EventList *events = new_event_list();

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Arquivo não encontrado: %s\n", filename);
        exit(1);
    }
    getline(&line, &len, fp);
    while (getline(&line, &len, fp) != -1) {
        Event *e = parse_line(line);
        add_event(events, e->time, e->type, e->pcb);
    }
    fclose(fp);
    return events;
}

Event *parse_line(char *line) {
    int pid;
    int arrival;
    int required_service;
    char iostarts[64];
    char iotypes[64];

    sscanf(line, "%d %d %d %s %s", &pid, &arrival, &required_service, iostarts, iotypes);
    
    PCB *pcb = new_process(pid, required_service);

    if (iostarts[0] != '-') {
        char *save_start;
        char *save_type;
        char *next_start = strtok_r(iostarts, ",", &save_start);
        char *next_type = strtok_r(iotypes, ",", &save_type);

        while (next_start != NULL) {
            add_io(pcb, io_from_s(next_type), atoi(next_start));
            next_start = strtok_r(NULL, ",", &save_start);
            next_type = strtok_r(NULL, ",", &save_type);
        }
    }

    return new_event(arrival, ARRIVAL, pcb);
}
 
IOType io_from_s(char *str) {
    if (str[0] == 'I') {
        return PRINTER;
    } else if (str[0] == 'D') {
        return DISK;
    } else if (str[0] == 'F') {
        return TAPE;
    } else {
        printf("Dispositivo de IO não reconhecido: %s\n", str);
        printf("Dispositivos reconhecidos: I, D, F\n");
        exit(1);
     }
}