#include <stdio.h>
#include <stdlib.h>

#include "sorts.h"
#include "times.h"
#include "utils.h"

#define TEST_COUNT      5
#define TEST_SIZE_COUNT 10


const uint32_t test_sizes[TEST_SIZE_COUNT] = {
  1000,
  2000,
  3000,
  4000,
  5000,
  6000,
  7000,
  8000,
  9000,
  10000
};

sort_context_t *ctx;

  static inline void
handle_options_input()
{
  int i;

  for(i=0; i<SORT_ARGS; i++){
    if(strcmp("", options[ctx->sort_index][i]) == 0)
      break;
    printf("%d: %s\n", i, options[ctx->sort_index][i]);
  }

  system("/bin/stty raw");
  char c;
  while(1){
    c = getchar() - 0x30;
    if(c <= i){
      ctx->sort_opt = c;
      return;
    }
  }
  system("/bin/stty cooked");
}

  static inline int
handle_presort_input()
{
  uint32_t size;

  if(scanf("%u", &size) != 1)
    return -1;

  if(size > ctx->size)
    return -1;

  ctx->sort_size = size;
  ctx->presort_proc = ((float)size)/((float)ctx->size);
  return 0;
}

  static inline char
handle_sort_input()
{
  system("/bin/stty raw");
  printf("0. Binary insertion sort \r\n");
  printf("1. Heap sort \r\n");
  printf("2. Shell sort \r\n");
  printf("3. Quick sort \r\n");
  char c = getchar();
  system("/bin/stty cooked");
  printf("\n");

  return c - 0x30;
}

  static inline char
handle_menu_input()
{
  system("/bin/stty raw");
  printf("0. Start algorithm\r\n");
  printf("1. Load array from file\r\n");
  printf("2. Generate array\r\n");
  printf("3. Print unsorted array\r\n");
  printf("4. Set sort type\r\n");
  printf("5. Print sorted array\r\n");
  printf("6. Check if array is sorted \r\n");
  printf("7. Print sorting times\r\n");
  printf("8. Pre-sort array\r\n");
  printf("9. Choose algorithm option\r\n");
  char c = getchar();
  system("/bin/stty cooked");
  printf("\n");
  return c;
}

  static void
main_loop()
{
  char c = '1';
  int res;
  while(c != 'q'){
    c = handle_menu_input();
    switch(c){
      case '0':
        execute_sort(ctx);
        break;
      case '1':
        if(parse_file(ctx) == -1)
          printf("Error while parsing file\n");
        break;
      case '2':
        uint32_t size;
        scanf("%u", &size);
        if(gen_rand_array(ctx, size) == -1)
          printf("Error reallocing\n");
        break;
      case '3':
        if(print_array(ctx) == -1)
          printf("Array not initalized\n");
        break;
      case '4':
        char choice = handle_sort_input();
        if(choice <= MAX_SORT)
          ctx->sort_index = (choice);
        break;
      case '5':
        if(print_sorted_array(ctx) == -1)
          printf("Sort array not initalized\n");
        break;
      case '6':
        res = check_if_sorted(ctx);
        if(res == -1){
          printf("Array not sorted\n");
        }else if(res == 0){
          printf("Array sorted\n");
        }else if(res == -2){
          printf("Array not initalized\n");
        }
        break;
      case '7':
        print_times(ctx);
        break;
      case '8':
        res = handle_presort_input();
        if(res == 0)
          printf("Wrong input\n");
        else
          presort(ctx);
        break;
      case '9':
        handle_options_input();
        break;
      default:
        break;
    }
  }
}

  void
do_test()
{
  for(int i=0; i<TEST_COUNT; i++){
    for(int j=0; j<TEST_SIZE_COUNT; j++){
      for(int k=0; k<4; k++){
        for(int o=0; o<4; o++){
          for(int l=1; l<4;l++){
            gen_rand_array(ctx, test_sizes[j]);
            ctx->sort_index = k;
            if(l != 1){
              ctx->sort_size = test_sizes[j]/l;
              ctx->presort_proc = 1.0f/(float)l;
              presort(ctx);
            }else{
              ctx->presort_proc = 0.0f;
            } 
            ctx->sort_opt = o;
            execute_sort(ctx);
          }
        }
      }
    }
  }
  print_times(ctx);
}

  int
main()
{
  init_sort_ctx(&ctx);
  setup_sort_list(&ctx);

  do_test();

  deinit_sort_ctx(&ctx);
  return 0;
}
