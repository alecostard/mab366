#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "process.h"
#include "event.h"

enum ProcessPriority {HIGH, LOW};

typedef enum ProcessPriority ProcessPriority;

int run_scheduler(EventList *events, int quantum);

#endif