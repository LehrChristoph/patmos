/*
    Small test program for the One-Way Shared Memory

    Author: Martin Schoeberl
    Copyright: DTU, BSD License
*/

#include <stdio.h>
#include <stdlib.h>
#include <machine/patmos.h>
#include <machine/spm.h>


#include "../../libcorethread/corethread.h"

#define CNT 4
#define ONEWAY_BASE *((volatile _SPM int *) 0xE8000000)
#define WORDS 256

// Shared data in main memory for the return value
volatile _UNCACHED static int field;
volatile _UNCACHED static int end_time;

// The main function for the other threads on the another cores
void work(void* arg) {

  // Measure execution time with the clock cycle timer
  volatile _IODEV int *timer_ptr = (volatile _IODEV int *) (PATMOS_IO_TIMER+4);
  volatile _SPM int *mem_ptr = (volatile _IODEV int *) (0xE8000000);

  *mem_ptr = 0xabcd;

  int id = get_cpuid();
  for (int i=0; i<CNT; ++i) {
    for (int j=0; j<WORDS; ++j) {
      *mem_ptr++ = id*0x10000 + i*0x100 + j;
    }
  }
}

int main() {

  // Pointer to the deadline device
  volatile _IODEV int *dead_ptr = (volatile _IODEV int *) PATMOS_IO_DEADLINE;
  // Measure execution time with the clock cycle timer
  volatile _IODEV int *timer_ptr = (volatile _IODEV int *) (PATMOS_IO_TIMER+4);
  volatile _SPM int *rxMem = (volatile _IODEV int *) (0xE8000000);

  for (int i=1; i<get_cpucnt(); ++i) {
    corethread_create(i, &work, NULL); 
  }

  printf("Number of cores: %d\n", get_cpucnt());
  for (int i=0; i<CNT; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%08x\n", rxMem[i*WORDS + j]);
    }
  }
  return 0;
}
