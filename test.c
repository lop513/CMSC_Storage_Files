// File for testing buffer.c in kernel space

#include "buffer.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

#define __NR_init_buffer_421 442
#define __NR_insert_buffer_421 443
#define __NR_print_buffer_421 444
#define __NR_delete_buffer_421 445

long init_buffer_421_syscall(void) {
  return syscall(__NR_init_buffer_421);
}

long insert_buffer_421_syscall(int i){
  return syscall(__NR_insert_buffer_421, i);
}

long print_buffer_421_syscall(void){
  return syscall(__NR_print_buffer_421);
}

long delete_buffer_421_syscall(void){
  return syscall(__NR_delete_buffer_421);
}

int main(int argc, char *argv[]){
  
  printf("Trying to enter data into a buffer before initialization: \n");
  insert_buffer_421_syscall(2);
  printf("\n");

  
  printf("Tests normal initialization, printing, insertion, and deletion: \n");
  init_buffer_421_syscall();
  
  printf("Prints initial buffer \n");
  print_buffer_421_syscall();
  
  printf("Insert data into each node increasing by one each time \n");
  for(int i = 1; i < 21; i++){
    insert_buffer_421_syscall(i);
  }

  printf("Prints buffer with new data in each node \n");
  print_buffer_421_syscall();

  printf("Finally delete the buffer\n");
  delete_buffer_421_syscall();
  printf("\n");

  
  printf("Trys to print buffer after deletion: \n");
  print_buffer_421_syscall();
  printf("\n");
  
  
  printf("Trying to enter data into a buffer after deletion: \n");
  insert_buffer_421_syscall(2);
  printf("\n");


  printf("Trying to initialize a buffer that already exists: \n");
  init_buffer_421_syscall();
  init_buffer_421_syscall();
  delete_buffer_421_syscall();
  printf("\n");

  
  printf("Trying to delete a deleted buffer: \n");
  delete_buffer_421_syscall();
  printf("\n");


  printf("Trying to print an uninitialized buffer: \n");
  print_buffer_421_syscall();
  printf("\n");


  printf("Trying to put data into a full buffer: \n");
  init_buffer_421_syscall();
  for(int i = 5; i < 25; i++){
    insert_buffer_421_syscall(i);
  }
  print_buffer_421_syscall();
  insert_buffer_421_syscall(1);
  delete_buffer_421_syscall();
  printf("\n");

  
  return 0;
}
