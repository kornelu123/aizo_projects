#pragma once

#include <time.h>

#define TIME_CONTEXT_FMT "%s sort no %d, time: %.16f[ms], size: %d, pre-sorting: %.02f, options: %s \n"

#define SORT_ARGS        4

static const char *opt_fmt[] = {
  "type : %s",
  "none%s",
  "gaps: %s",
  "pivot : %s"
};

static const char *types[] = {
  "uint32_t",
  "float",
  "",
  ""
};

static const char *none[] = {
  " ",
  "",
  "",
  ""
};

static const char *gaps[] = {
  "pow2",
  "curie",
  "",
  ""
};

static const char *pivots[] = {
  "left",
  "right",
  "mid",
  "random"
};

static const char *names[] = {
  "Insertion",
  "Heap",
  "Shell",
  "Quick"
};

static const char **options[] = {
  types,
  none,
  gaps,
  pivots
};

typedef struct {
  char          *str;
  uint32_t      msg_len;          
} time_context_t;
