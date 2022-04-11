// This file is the implementation of the semaphore in user space

#include "buffer_sem.h"
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>


static bb_buffer_421_t buff;
struct bb_node_421 *head;
struct bb_node_421 *tail;
int init_check = 0;

static sem_t empty;
static sem_t full;
static sem_t mutex;


long init_buffer_421(void){
  
  struct bb_node_421 *ptr;
  struct bb_node_421 *newNode;
  int i;
  int j;
  
  if(init_check == 0){

    // Initalize mutex and semaphores 
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, SIZE_OF_BUFFER);
    sem_init(&full, 0, 0);
    
    head = malloc(sizeof(struct bb_node_421));
    buff.write = head;
    for(i = 0; i < DATA_LENGTH; i++){
      head->data[i] = '\0';
    }
    ptr = buff.write;
    for(i = 0; i < SIZE_OF_BUFFER - 1; i++){

      newNode = malloc(sizeof(struct bb_node_421));
      for(j = 0; j < DATA_LENGTH; j++){
	newNode->data[j] = '\0';
      }
      ptr->next = newNode;
      ptr = ptr->next;
      
    }
    
    ptr->next = buff.write;
    tail = buff.write;
    head = buff.write;
    buff.read = head;
    buff.length = 0;
    
    printf("Buffer initialized \n");
    init_check = 1;
    return 0;
    
  }
  
  else {
    printf("Failed to initialize buffer, buffer already exists \n");
    return -1;
  }
  
}

// The Producer
long enqueue_buffer_421(char *data){
  int i;
  int num;


  sem_wait(&mutex); // Lock the mutex since we could be producing

  
  if(init_check == 1) {
    if(buff.length < 20){

      sem_wait(&empty);
      printf(":: Enqueueing element into buffer. :: \n");
      num = data[0];
      printf("%c%c%c%c%c%c%c%c%c%c... \n", num, num, num, num, num, num, num, num, num, num);

      
      for(i = 0; i < DATA_LENGTH; i++){
	buff.write->data[i] = *data;
      }
     
      
      buff.length++;
      printf("%d item(s) in buffer. \n", buff.length);
      buff.write = buff.write->next;

      sem_post(&mutex);  // Unlock the mutex since we are done producing 
      sem_post(&full);  // Signal to consumer that buffer has some data that they can consume
      
      return 0;
    }
    sem_post(&mutex);  // Unlock the mutex since we are done producing 
    //sem_post(&full); 
   
    printf("Failed to enqueue data into buffer, buffer is full \n");
   
    return -1;
  }

  else {
    sem_post(&mutex);  // Unlock the mutex since we are done producing
    //sem_post(&full);
    
    printf("Failed to enqueue data into buffer, buffer does not exist \n");
    
    return -1;
  }
  
}

// The Consumer
long dequeue_buffer_421(char *data){
  int i;
  int num;

  sem_wait(&mutex);  // Lock the mutex since we could be consuming

  //curr = head;
  
  if(init_check == 1){
    if(buff.length > 0){
      
      sem_wait(&full);
      printf(":: Dequeueing element from buffer. :: \n");
      num = data[0];
      printf("%c%c%c%c%c%c%c%c%c%c... \n", num, num, num, num, num, num, num, num, num, num);
      
      
      for(i = 0; i < DATA_LENGTH; i++){
	data = &buff.read->data[i];
	buff.read->data[i] = '\0';
      }
      //buff.write = buff.read;
      /*
      // Main deletion code 
      while(curr->data[0] != data[0]){
      
	if(curr->next == head){
	  printf("Node is not found\n");
	  break;
	}
	prev = curr;
	curr = curr->next;
      }
      */
      buff.length--;
      printf("%d item(s) in buffer. \n", buff.length);
      buff.read = buff.read->next;

      sem_post(&mutex);  // Unlock the mutex since we are done consuming 
      sem_post(&empty);  // Signal to producer that buffer has some empty slots allowing them to produce
      
      return 0;
    }
    sem_post(&mutex);  // Unlock the mutex since we are done consuming 
    //sem_post(&empty); 
    
    printf("Failed to dequeue data from buffer, buffer is empty  \n");
    
    return -1;
  }

  else {
    sem_post(&mutex);  // Unlock the mutex since we are done consuming
    //sem_post(&empty); 
    
    printf("Failed to dequeue data from buffer, buffer does not exist \n");

    return -1;
  }
}


long delete_buffer_421(void){
  int counter;
  int i;
  
  struct bb_node_421 *temp;
  
  if(init_check == 1){
    
    counter = SIZE_OF_BUFFER;
    
    while(counter > 0){

      for(i = 0; i < DATA_LENGTH; i++){
	head->data[i] = '\0';
      }
      
      temp = head->next;
      
      free(head);
      head = temp;
      
      counter--;
    }
    
   
    buff.length = 0;

    // Delete the semaphores and mutex
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    printf("Buffer deleted \n");
    init_check = 0;
    return 0;
  }

  else {
    printf("Failed to delete buffer, buffer does not exist \n");
    return -1;
  }

}
 

// Only in user space
void print_semaphores(void){
  int value;

  
  if(init_check == 1){

    printf("\n");
    sem_getvalue(&mutex, &value);
    printf("sem_t mutex = %d \n", value);
    
    sem_getvalue(&full, &value);
    printf("Sem_t fill-count = %d \n", value);

    sem_getvalue(&empty, &value);
    printf("Sem_t empty-count = %d \n", value); 
    printf("\n");
    
    return;
  }

  else{
    printf("Failed to print sems, sems do not exist \n");  
    return;
  }
 
}

