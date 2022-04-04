// File for testing buffer_user.c in user space

#include "buffer.h"

#include <stdio.h>


int main(){

  printf("Trying to enter data into a buffer before initialization: \n");
  insert_buffer_421(2);
  printf("\n");

  
  printf("Tests normal initialization, printing, insertion, and deletion: \n");
  init_buffer_421();
  
  printf("Prints initial buffer \n");
  print_buffer_421();
  
  printf("Insert data into each node increasing by one each time \n");
  for(int i = 1; i < 21; i++){
    insert_buffer_421(i);
  }

  printf("Prints buffer with new data in each node \n");
  print_buffer_421();

  printf("Finally delete the buffer\n");
  delete_buffer_421();
  printf("\n");

  
  printf("Trys to print buffer after deletion: \n");
  print_buffer_421();
  printf("\n");
  
  
  printf("Trying to enter data into a buffer after deletion: \n");
  insert_buffer_421(2);
  printf("\n");


  printf("Trying to initialize a buffer that already exists: \n");
  init_buffer_421();
  init_buffer_421();
  delete_buffer_421();
  printf("\n");

  
  printf("Trying to delete a deleted buffer: \n");
  delete_buffer_421();
  printf("\n");


  printf("Trying to print an uninitialized buffer: \n");
  print_buffer_421();
  printf("\n");


  printf("Trying to put data into a full buffer: \n");
  init_buffer_421();
  for(int i = 5; i < 25; i++){
    insert_buffer_421(i);
  }
  print_buffer_421();
  insert_buffer_421(1);
  delete_buffer_421();
  printf("\n");

  
  return 0;
}
