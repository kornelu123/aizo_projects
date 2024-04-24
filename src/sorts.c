#include <time.h>
#include <stdlib.h>

#include "sorts.h"

void (*do_insert[2])(sort_context_t *);
void (*do_shell[2])(sort_context_t *);

#define SHELL_CURIE_SIZE 8

const uint32_t shell_curie[SHELL_CURIE_SIZE] = {
  701,
  301,
  132,
  57,
  23,
  10,
  4,
  1
};

  static inline int
bin_search_u32(sort_context_t *ctx, int l, int r, uint32_t s)
{
  while(l < r){
    int mid = (l + r)/2;

    if(ctx->sort_arr->u_arr[mid] <= s)
      l = mid + 1;
    else
      r = mid;
  }
  return l;
}

  static inline int
bin_search_fl(sort_context_t *ctx, int l, int r, float s)
{
  while(l < r){
    int mid = (l + r)/2;

    if(ctx->sort_arr->fl_arr[mid] <= s){
      l = mid + 1;
    } else {
      r = mid;
    }
  }
  return l;
}

  int
execute_sort(sort_context_t *ctx)
{
  if(strcmp("",options[ctx->sort_index][ctx->sort_opt]) == 0)
    return -1;

  const uint32_t arr_size = ctx->size*sizeof(uint32_t);
  ctx->sort_arr->u_arr = realloc(ctx->sort_arr->u_arr, arr_size);
  memcpy(ctx->sort_arr->u_arr, ctx->arr, arr_size);

  clock_t start = clock();
  ctx->sorts[ctx->sort_index](ctx);
  clock_t end = clock();
  add_time(start, end, ctx);

  return 0;
}

  void
presort(sort_context_t *ctx)
{
  memcpy(ctx->sort_arr->u_arr, ctx->arr, ctx->size);
  ctx->sorts[QUICK_SORT](ctx);
  memcpy(ctx->arr, ctx->sort_arr->u_arr, ctx->size);
  print_array(ctx);
  ctx->sort_size = ctx->size;
}

  static inline void
swap(uint32_t *a, uint32_t *b)
{
  uint32_t temp = *a;
  *a = *b;
  *b = temp;
}

  static inline void
insert_sort_fl(sort_context_t *ctx)
{
  for(uint32_t i=1; i<ctx->sort_size; i++){
    float elem = ctx->sort_arr->fl_arr[i];
    int index = bin_search_fl(ctx, 0, i, elem);

    int j = i;
    while(j > index){
      ctx->sort_arr->fl_arr[j] = ctx->sort_arr->fl_arr[j - 1];
      j--;
    }

    ctx->sort_arr->fl_arr[j] = elem;
  }
}

  static inline void
insert_sort_u32(sort_context_t *ctx)
{
  for(uint32_t i=1; i<ctx->sort_size; i++){
    uint32_t elem = ctx->sort_arr->u_arr[i];
    int index = bin_search_u32(ctx, 0, i, elem);

    int j = i;
    while(j > index){
      ctx->sort_arr->u_arr[j ] = ctx->sort_arr->u_arr[j - 1];
      j--;
    }

    ctx->sort_arr->u_arr[j] = elem;
  }
}

  static inline int
insert_sort(sort_context_t *ctx)
{
  do_insert[ctx->sort_opt](ctx);
  return 0;
}

  static inline void
heapify(sort_context_t *ctx, uint32_t n, uint32_t i)
{
  uint32_t lar = i;
  uint32_t l = 2 * i + 1;
  uint32_t r = 2 * i + 2;

  if ( l < n && ctx->sort_arr->u_arr[l] > ctx->sort_arr->u_arr[lar] )
    lar = l;

  if ( r < n && ctx->sort_arr->u_arr[r] > ctx->sort_arr->u_arr[lar] )
    lar = r;

  if( lar != i ){
    swap(&(ctx->sort_arr->u_arr[i]), &(ctx->sort_arr->u_arr[lar]));

    heapify(ctx, n, lar);
  }
}

  static inline int
heap_sort(sort_context_t *ctx)
{
  for(int i=(ctx->sort_size / 2) - 1; i >= 0; i--){
    heapify(ctx, ctx->sort_size, i);
  }

  for(int i = ctx->sort_size - 1; i >= 0; i--){
    swap(&(ctx->sort_arr->u_arr[0]), &(ctx->sort_arr->u_arr[i]));

    heapify(ctx, i, 0);
  }
  return 0;
}

  int
shell_sort(sort_context_t *ctx)
{
  do_shell[ctx->sort_opt](ctx);
  return 0;
}

  void
shell_sort_curie(sort_context_t *ctx)
{
  for(uint32_t i = 0; i < SHELL_CURIE_SIZE; i++){
    for(uint32_t j = shell_curie[i]; j < ctx->sort_size; j++){
      uint32_t temp = ctx->sort_arr->u_arr[j];
      uint32_t k;
      for(k = j; k >= shell_curie[i] && ctx->sort_arr->u_arr[k-shell_curie[i]] > temp; k-= shell_curie[i]){
        ctx->sort_arr->u_arr[k] = ctx->sort_arr->u_arr[k-shell_curie[i]];
      }
      ctx->sort_arr->u_arr[k] = temp;
    }
  }
}

  void
shell_sort_2n(sort_context_t *ctx)
{
  for(uint32_t i = ctx->sort_size/2; i > 0; i/=2){
    for(uint32_t j = i; j < ctx->sort_size; j++){
      uint32_t temp = ctx->sort_arr->u_arr[j];
      uint32_t k;
      for(k = j; k >= i && ctx->sort_arr->u_arr[k-i] > temp; k-= i){
        ctx->sort_arr->u_arr[k] = ctx->sort_arr->u_arr[k-i];
      }
      ctx->sort_arr->u_arr[k] = temp;
    }
  }
}

  static inline uint32_t
partition(sort_context_t *ctx, uint32_t l, uint32_t r)
{
  uint32_t piv = ctx->get_pivot[ctx->sort_opt](ctx->sort_arr->u_arr, l, r);

  uint32_t left = l, right = r;
  while(1){
    while(ctx->sort_arr->u_arr[left]<piv) ++left;
    while(ctx->sort_arr->u_arr[right]>piv) --right;

    if(left < right){
      swap(&(ctx->sort_arr->u_arr[right--]), &(ctx->sort_arr->u_arr[left++]));
    } else {
      if (right == r) right--;
      return right;
    }
  }
}

  static inline void
qsort_intern(sort_context_t *ctx, uint32_t l, uint32_t r)
{
  if(l >= r)
    return;

  uint32_t par = partition(ctx, l, r);
  qsort_intern(ctx, l, par);
  qsort_intern(ctx, par + 1, r);
}

  static inline int
quick_sort(sort_context_t *ctx)
{
  qsort_intern(ctx, 0, ctx->sort_size - 1);

  return 0;
}

  uint32_t
get_left_piv(uint32_t *data, uint32_t l, uint32_t r)
{
  (void) r;
  return data[l];
}

  uint32_t
get_right_piv(uint32_t *data, uint32_t l, uint32_t r)
{
  (void) l;
  return data[r];
}

  uint32_t
get_mid_piv(uint32_t *data, uint32_t l, uint32_t r)
{
  return data[(l + r)/2];
}

  uint32_t
get_rand_piv(uint32_t *data, uint32_t l, uint32_t r)
{
  return data[l + (rand()% (r - l + 1))];
}

  static inline void
setup_qsort_options(sort_context_t *ctx)
{
  srand(time(NULL));

  ctx->get_pivot[QUICK_SORT_PIVOT_LEFT]     = &get_left_piv;
  ctx->get_pivot[QUICK_SORT_PIVOT_RIGHT]    = &get_right_piv;
  ctx->get_pivot[QUICK_SORT_PIVOT_MID]      = &get_mid_piv;
  ctx->get_pivot[QUICK_SORT_PIVOT_RAND]     = &get_rand_piv;

  ctx->sort_opt = QUICK_SORT_PIVOT_LEFT;
}

  static inline void
setup_isort_options()
{
  do_insert[0] = &insert_sort_u32;
  do_insert[1] = &insert_sort_fl;
}

  static inline void
setup_shsort_options()
{
  do_shell[0] = &shell_sort_2n;
  do_shell[1] = &shell_sort_curie;
}

  void
setup_sort_list(sort_context_t **ctx)
{
  (*ctx)->sorts[INSERT_SORT]   = &insert_sort;
  (*ctx)->sorts[HEAP_SORT]     = &heap_sort; 
  (*ctx)->sorts[SHELL_SORT]    = &shell_sort;
  (*ctx)->sorts[QUICK_SORT]    = &quick_sort;

  setup_qsort_options(*ctx);
  setup_isort_options();
  setup_shsort_options();
}

  int
check_if_sorted(sort_context_t *ctx)
{
  if(ctx->sort_arr->u_arr == NULL)
    return -2;

  for(uint32_t i=0; i<ctx->size - 1; i++){
    if(ctx->sort_arr->u_arr[i] > ctx->sort_arr->u_arr[i+1])
      return -1;
  }

  return 0;
}
