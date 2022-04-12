// This file is the implementation of the semaphore in kernel space

#include "buffer_sem.h"
/*
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
*/
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/semaphore.h>
#include <linux/init.h>


static bb_buffer_421_t buff;
struct bb_node_421 *head_sem;
struct bb_node_421 *tail_sem;
int init_sem_check = 0;

struct semaphore empty;
struct semaphore full;
struct semaphore mutex;


SYSCALL_DEFINE0(init_buffer_sem_421){
  
  struct bb_node_421 *ptr;
  struct bb_node_421 *newNode;
  int i;
  int j;
  
  if(init_sem_check == 0){

    // Initalize mutex and semaphores 
    sema_init(&mutex, 1);
    sema_init(&empty, SIZE_OF_BUFFER);
    sema_init(&full, 0);
    
    head_sem = kmalloc(sizeof(struct bb_node_421), GFP_KERNEL);
    buff.write = head_sem;
    
    for(i = 0; i < DATA_LENGTH; i++){
      head_sem->data[i] = '\0';
    }
    
    ptr = buff.write;
    for(i = 0; i < SIZE_OF_BUFFER - 1; i++){

      newNode = kmalloc(sizeof(struct bb_node_421), GFP_KERNEL);
      
      for(j = 0; j < DATA_LENGTH; j++){
	newNode->data[j] = '\0';
      }
      
      ptr->next = newNode;
      ptr = ptr->next;
      
    }
    
    ptr->next = buff.write;
    tail_sem = buff.write;
    head_sem = buff.write;
    buff.read = head_sem;
    buff.length = 0;
    
    printk("Buffer initialized \n");
    init_sem_check = 1;
    return 0;
    
  }
  
  else {
    printk("Failed to initialize buffer, buffer already exists \n");
    return -1;
  }
  
}

// The Producer
SYSCALL_DEFINE1(enqueue_buffer_sem_421, char *, data){
  int num;

  num = 0;
  
  
  if(init_sem_check == 1) {
    
      
    down(&empty);
    down(&mutex);
      
    printk(":: Enqueueing element into buffer. :: \n");
      
      
    //memcpy(buff.write->data, data, DATA_LENGTH);
    copy_to_user(buff.write->data, data, DATA_LENGTH);
     
    num = data[0];
    printk("%c%c%c%c%c%c%c%c%c%c... \n", num, num, num, num, num, num, num, num, num, num);

      
    buff.length++;
    printk("%d item(s) in buffer. \n", buff.length);
    buff.write = buff.write->next;

    up(&mutex);  // Unlock the mutex since we are done producing 
    up(&full);  // Signal to consumer that buffer has some data that they can consume
      
    return 0;
  
  }

  else {
    
    up(&mutex);  // Unlock the mutex since we are done producing
    
    printk("Failed to enqueue data into buffer, buffer does not exist \n");
    
    return -1;
  }
  
}

// The Consumer
SYSCALL_DEFINE1(dequeue_buffer_sem_421, char*, data){
  int num;

  num = 0;

  if(init_sem_check == 1){
    
      
    down(&full);
    down(&mutex);
      
    printk(":: Dequeueing element from buffer. :: \n");
	
      
    //memcpy(data, buff.read->data, DATA_LENGTH);
    copy_to_user(data, buff.read->data, DATA_LENGTH);
      

    num = data[0];
    printk("%c%c%c%c%c%c%c%c%c%c... \n", num, num, num, num, num, num, num, num, num, num);
      
      
    buff.length--;
    printk("%d item(s) in buffer. \n", buff.length);
    buff.read = buff.read->next;

    up(&mutex);  // Unlock the mutex since we are done consuming 
    up(&empty);  // Signal to producer that buffer has some empty slots allowing them to produce
    
    return 0;
    
  }

  else {
    up(&mutex);  // Unlock the mutex since we are done consuming
    
    
    printk("Failed to dequeue data from buffer, buffer does not exist \n");

    return -1;
  }
}


SYSCALL_DEFINE0(delete_buffer_sem_421){
  int counter;
  int i;
  
  struct bb_node_421 *temp;

  
  if(init_sem_check == 1){
    
    counter = SIZE_OF_BUFFER;
    
    while(counter > 0){

      for(i = 0; i < DATA_LENGTH; i++){
	head_sem->data[i] = '\0';
      }
      
      temp = head_sem->next;
      
      kfree(head_sem);
      head_sem = temp;
      
      counter--;
    }
    
   
    buff.length = 0;

    // Delete the semaphores and mutex
    //sema_destroy(&mutex);
    //sema_destroy(&empty);
    //sema_destroy(&full);
    
    printk("Buffer deleted \n");
    init_sem_check = 0;
    return 0;
  }

  else {
    printk("Failed to delete buffer, buffer does not exist \n");
    return -1;
  }

}
 
