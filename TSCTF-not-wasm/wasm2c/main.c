#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Uncomment this to define fac_init and fac_Z_facZ_ii instead. */
/* #define WASM_RT_MODULE_PREFIX fac_ */

#include "enc.h"

// wasm2c ../enc.wasm -o enc.c
// gcc -fno-lto ./main.c ./enc.c ./wasm-rt-impl.c -o main
// strip ./main -N check -N main -N memory -N inp -N Z_inpZ_i -N Z_checkZ_iv

extern wasm_rt_memory_t memory;

int main(int argc, char** argv) {

  /* Initialize the fac module. Since we didn't define WASM_RT_MODULE_PREFIX,
  the initialization function is called `init`. */

  init();
//   Z__initializeZ_vv();

    puts("Tell me the flag.");
  
  // 输入到Z_inpZ_i;
  int ret = scanf("%16s", &(memory.data[*Z_inpZ_i]));
  if (ret != 1) {
      puts("What?");
      exit(-1);
  }
//   for(int i=0;i<17;i++) {
//       printf("%02x ", (memory.data[*Z_inpZ_i+i]));
//   }
  
  /* Call `check`, using the mangled name. */
  u32 result = Z_checkZ_iv();
  if(result) {
      puts("Well, you are right.");
  } else {
      puts("You forgot the flag.");
  }
//   1680 cmp
//   for(int i=0;i<17;i++) {
//       printf("%02x ", (memory.data[1824+i]));
//   }

  return 0;
}
