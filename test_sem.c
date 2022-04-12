// File for testing buffer_sem.c in kernel space

#include "buffer_sem.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

#define __NR_init_buffer_sem_421 446
#define __NR_enqueue_buffer_sem_421 447
#define __NR_dequeue_buffer_sem_421 448
#define __NR_delete_buffer_sem_421 449


long init_buffer_sem_421_syscall(void){
  return syscall(__NR_init_buffer_sem_421);
}

long enqueue_buffer_sem_421_syscall(char* data){
  return syscall(__NR_enqueue_buffer_sem_421, *data);
}

long dequeue_buffer_sem_421_syscall(char* data){
  return syscall(__NR_dequeue_buffer_sem_421, *data);
}

long delete_buffer_sem_421_syscall(void){
  return syscall(__NR_delete_buffer_sem_421);
}

void *producer(){
  char *prod_data[DATA_LENGTH];
  char curr[1];
  int num_check;
  int i;
  int j;
  int max;
  
  
  max = 10;
  num_check = 0;
  curr[0] = 0;
  curr[0] = num_check + '0';

  for(i = 0; i < 1000; i++){

    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }

    for(j = 0; j < DATA_LENGTH; j++){
      prod_data[j] = curr;
    }
    

    usleep(rand()%max);
    if(enqueue_buffer_sem_421_syscall(*prod_data) == 0){
      num_check++;
    }
    
    curr[0] = num_check + '0';
    
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

  
  max = 10;
  num_check = 0;
  curr[0] = 0;
  curr[0] = num_check + '0';

  for(i = 0; i < 1000; i++){
    
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }

    
    for(j = 0; j < DATA_LENGTH; j++){
      con_data[j] = curr;
    }

    
    
    usleep(rand()%max);
    if(dequeue_buffer_sem_421_syscall(*con_data) == 0){
      num_check++;
    }
    

    curr[0] = num_check + '0';
    
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

  
  printf("Testing Producer and Consumer threads \n");
  printf("Scenario 1: An empty buffer \n");
  
  init_buffer_sem_421_syscall();
  
  srand(time(0));
  pthread_create(&pro, 0, producer, NULL);
  srand(time(0));
  pthread_create(&con, 0, consumer, NULL);

  
  pthread_join(pro, NULL);
  pthread_join(con, NULL);
  
  delete_buffer_sem_421_syscall();
  printf("\n");


  printf("Scenario 2: A full buffer \n");
  init_buffer_sem_421_syscall();

  for(i = 0; i < 20; i++){
  
    if(num_check > 9) {
      num_check = 0;
      curr[0] = num_check + '0';
    }
    
    for(j = 0; j < DATA_LENGTH; j++){
      curr_num[j] = curr;
    }
    
    enqueue_buffer_sem_421_syscall(*curr_num);
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
  
  delete_buffer_sem_421_syscall();
  printf("\n");
  
  return 0;
}


