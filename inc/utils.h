#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "times.h"
#include "sorts.h"

#define MAX_FNAME_LEN 64
#define MAX_NUM_LEN   64

typedef union {
  float         *fl_arr;
  uint32_t      *u_arr;
} array;

typedef struct {
  uint32_t          *arr;
  uint32_t          sort_size;
  uint32_t          size;
  array             *sort_arr;

  int       (*sorts[4])(sort_context_t);
  uint32_t  (*get_pivot[4])(uint32_t *, uint32_t, uint32_t);

  time_context_t    *ms_time;
  int               time_count;

  float             presort_proc;

  uint8_t           sort_opt;
  uint8_t           sort_index;
}sort_context_t;


  static inline int
parse_int(char **lineptr)
{
  int res = 0;
  if(sscanf(*lineptr, "%d", &res) != 1)
    return -1;

  return res;
}

  static inline int
parse_file(sort_context_t *ctx)
{
  int res = 0;
  char fname[MAX_FNAME_LEN];
  scanf("%s", fname);
  FILE *fptr;
  if((fptr = fopen(fname, "r")) == NULL){
    res = -1;
    printf("Error opening file\n");
  } else {
    char *lineptr  = NULL;
    size_t size    = 0;
    ssize_t chars  = 0;
    if((chars = getline(&lineptr, &size, fptr)) == -1){
      perror("getline:");
    }else{
      ctx->size = parse_int(&lineptr);
      ctx->sort_size = ctx->size;
      ctx->arr  = realloc(ctx->arr, ctx->size*sizeof(uint32_t));
      for(uint32_t i=0; i<ctx->size && getline(&lineptr, &size, fptr) > 0; i++){
        ctx->arr[i] = parse_int(&lineptr);
      }
    }
    free(lineptr);
  }
  return res;
}

  static inline int
print_array(sort_context_t *ctx)
{
  if(ctx->arr == NULL)
    return -1;

  for(uint32_t i=0; i<ctx->size; i++){
    printf("%08X", ctx->arr[i]);
    if(i != ctx->size - 1)
      printf(":");
  }
  printf("\n\n");

  return 0;
}

  static inline int
print_sorted_array(sort_context_t *ctx)
{
  if(ctx->sort_arr->u_arr== NULL)
    return -1;

  for(uint32_t i=0; i<ctx->size; i++){
    printf("%08X", ctx->sort_arr->u_arr[i]);
    if(i != ctx->size - 1)
      printf(":");
  }
  printf("\n\n");

  return 0;
}

  static inline int
gen_rand_array(sort_context_t *ctx)
{
  scanf("%u", &(ctx->size));
  ctx->sort_size = ctx->size;
  ctx->arr  = realloc(ctx->arr, ctx->size*sizeof(uint32_t));
  if(ctx->arr == NULL)
    return -1;

  for(uint32_t i=0; i<ctx->size; i++){
    ctx->arr[i] = (uint32_t) rand();
  }

  return 0;
}

  static inline void
init_sort_ctx(sort_context_t **ctx)
{
  *ctx                  = (sort_context_t *) calloc (1, sizeof(sort_context_t));
  if(*ctx == NULL)
    perror("calloc");

  (*ctx)->sort_arr              = calloc(1, sizeof(array));
  (*ctx)->arr                   = NULL;
  (*ctx)->sort_arr->u_arr       = NULL;
  (*ctx)->ms_time               = NULL;
}

  static inline void
deinit_time_ctxs(sort_context_t *ctx)
{
  for(int i=0; i<ctx->time_count; i++){
    free(ctx->ms_time[i].str);
    free(&(ctx->ms_time[i]));
  }
}

  static inline void
deinit_sort_ctx(sort_context_t **ctx)
{
  deinit_time_ctxs(*ctx);

  free((*ctx)->sort_arr->u_arr);
  free((*ctx)->arr);
  free((*ctx)->sort_arr);
  free(*ctx);
}

  static inline void
set_time_context_text(time_context_t *tctx, sort_context_t *sctx, float time)
{
  char opt[128];
  sprintf(opt, opt_fmt[sctx->sort_index], options[sctx->sort_index][sctx->sort_opt]);

  char msg[256];
  const int size = sprintf(msg, TIME_CONTEXT_FMT, names[sctx->sort_index], sctx->time_count + 1, time, sctx->size, sctx->presort_proc, opt);

  tctx->str = (char *)malloc(size + 1);
  strcpy(tctx->str, msg);

  tctx->msg_len = size;
}

  static inline int
add_time(clock_t start, clock_t end, sort_context_t *ctx)
{
  const float time = 1000*(((float)start)/((float)end)/CLOCKS_PER_SEC);

  time_context_t *tctx = malloc(sizeof(time_context_t));
  if(tctx == NULL)
    return -1;

  set_time_context_text(tctx, ctx, time);

  ctx->ms_time = (time_context_t *) reallocarray (ctx->ms_time, ++ctx->time_count, sizeof(time_context_t));
  if(ctx->ms_time == NULL)
    return -1;

  ctx->ms_time[ctx->time_count - 1] = *tctx;
  free(tctx);
  return 0;
}

  static inline void
print_times(sort_context_t *ctx)
{
  for(int i=0; i< ctx->time_count; i++){
    printf(ctx->ms_time[i].str);
  }
}
