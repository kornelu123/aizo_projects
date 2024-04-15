#pragma once

#define INSERT_SORT 0
#define HEAP_SORT   1
#define SHELL_SORT  2
#define QUICK_SORT  3

#define MAX_SORT QUICK_SORT

#define QUICK_SORT_PIVOT_LEFT   0
#define QUICK_SORT_PIVOT_RIGHT  1
#define QUICK_SORT_PIVOT_MID    2
#define QUICK_SORT_PIVOT_RAND   3

#define MAX_QUICK_SORT_LEVELS   300

#include <time.h>
#include <stdlib.h>

#include "utils.h"
#include "times.h"

int execute_sort(sort_context_t *ctx);

void setup_sort_list(sort_context_t **ctx);

int check_if_sorted(sort_context_t *ctx);

void presort(sort_context_t *ctx);
