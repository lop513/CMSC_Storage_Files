// File for testing buffer_user_sem.c in user space

#include "buffer_sem.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

void *producer(){
  char *prod_data[DATA_LENGTH];
  char curr[1];
  int num_check;
  int i;
  int j;
  int max;

  //srand(time(NULL));
  
  max = 1000;
  num_check = 0;
  curr[0] = 0;
  curr[0] = num_check + '0';

  for(i = 0; i < 10; i++){

    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }

    for(j = 0; j < DATA_LENGTH; j++){
      prod_data[j] = curr;
    }
    
    //printf("%d \n", rand()%max);    
    //for(j = 0; j < rand()%max; j++){
     
    //}
    usleep(rand()%max);
    if(enqueue_buffer_421(*prod_data) == 0){
      num_check++;
    }
    
    curr[0] = num_check + '0';
    print_semaphores();
  }
  
  return 0;
}


void *consumer(){
  char *con_data[DATA_LENGTH];
  char curr[1];
  int num_check;
  int i;
  int j;
  int max;

  //srand(time(0));
  
  max = 1000;
  num_check = 0;
  curr[0] = 0;
  curr[0] = num_check + '0';

  for(i = 0; i < 10; i++){
    
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }

    
    for(j = 0; j < DATA_LENGTH; j++){
      con_data[j] = curr;
    }

    
    //printf("%d \n", rand()%max);
    //for(j = 0; j < rand()%max; j++){
      
    //}
    usleep(rand()%max);
    if(dequeue_buffer_421(*con_data) == 0){
      num_check++;
    }
    

    curr[0] = num_check + '0';
    print_semaphores();
  }
  
  return 0;
}


int main(){
  int num_check;
  char curr[1];
  char *curr_num[DATA_LENGTH];
  pthread_t pro;
  pthread_t con;
  int i;
  int j;

  
  
  num_check = 0;
  curr[0] = 0;
  curr[0] = num_check + '0';
  

  /*
  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = curr;
  }
  
  printf("Trying to enter data into a buffer before initialization: \n");
  enqueue_buffer_421(*curr_num);
  printf("\n");

  
  printf("Tests normal initialization, printing, insertion, and deletion: \n");
  init_buffer_421();
  
  printf("Prints initial buffer \n");
  print_semaphores();
  
  printf("Insert data into each node increasing by one each time \n");
  
  for(i = 0; i < 20; i++){
  
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }
    
    for(j = 0; j < DATA_LENGTH; j++){
      curr_num[j] = curr;
    }
    
    enqueue_buffer_421(*curr_num);
    num_check++;
    
    curr[0] = num_check + '0';
  }

  printf("Prints buffer with new data in each node \n");
  print_semaphores();

  printf("Finally delete the buffer\n");
  delete_buffer_421();
  printf("\n");

  
  printf("Trys to print buffer after deletion: \n");
  print_semaphores();
  printf("\n");
  
  
  printf("Trying to enter data into a buffer after deletion: \n");
  enqueue_buffer_421(0);
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
  print_semaphores();
  printf("\n");


  printf("Trying to put data into a full buffer: \n");
  init_buffer_421();
  
  for(i = 0; i < 20; i++){
    
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }
    
    for(j = 0; j < DATA_LENGTH; j++){
      curr_num[j] = curr;
    }
    
    enqueue_buffer_421(*curr_num);
    num_check++;
    curr[0] = num_check + '0';
  }

  print_semaphores();
  enqueue_buffer_421(*curr_num);
  delete_buffer_421();
  printf("\n");
  */

  printf("Testing to match semaphore Scenario 1: An empty buffer, manually controlled \n");
  init_buffer_421();
  
  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "5";
  }
  enqueue_buffer_421(*curr_num);
  
  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "6";
  }
  enqueue_buffer_421(*curr_num);
 
  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "5";
  }
  dequeue_buffer_421(*curr_num);

  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "6";
  }
  dequeue_buffer_421(*curr_num);

  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "7";
  }
  enqueue_buffer_421(*curr_num);

  for(i = 0; i < DATA_LENGTH; i++){
    curr_num[i] = "7";
  }
  dequeue_buffer_421(*curr_num);
  
  delete_buffer_421();
  printf("\n");

  
  printf("Testing Producer and Consumer threads \n");
  printf("Scenario 1: An empty buffer \n");
  
  init_buffer_421();

  print_semaphores();
  
  srand(time(0));
  pthread_create(&pro, 0, producer, NULL);
  srand(time(0));
  pthread_create(&con, 0, consumer, NULL);

  
  pthread_join(pro, NULL);
  pthread_join(con, NULL);
  
  print_semaphores();
  
  delete_buffer_421();
  printf("\n");


  printf("Scenario 2: A full buffer \n");
  init_buffer_421();

  for(i = 0; i < 20; i++){
  
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }
    
    for(j = 0; j < DATA_LENGTH; j++){
      curr_num[j] = curr;
    }
    
    enqueue_buffer_421(*curr_num);
    num_check++;
    
    curr[0] = num_check + '0';
  }

  printf("Buffer is now full \n");
  
  srand(time(0));
  pthread_create(&pro, 0, producer, NULL);
  srand(time(0));
  pthread_create(&con, 0, consumer, NULL);

  
  pthread_join(pro, NULL);
  pthread_join(con, NULL);

  print_semaphores();
  
  delete_buffer_421();
  printf("\n");
  
  return 0;
}


