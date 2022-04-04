// This file is the implementation of the buffer in user space

#include "buffer.h"
#include <stdio.h>
#include <malloc.h>

struct ring_buffer_421 buff;
struct node_421 *head = NULL;
struct node_421 *tail = NULL;

long init_buffer_421(void){
  
  if(head == NULL){
    
    buff.write = (struct node_421*) malloc(sizeof(struct node_421));
    buff.write->data = 0;
    buff.write->next = NULL;
   
    
    
    struct node_421 *ptr = buff.write;

    for(int i = 0; i < SIZE_OF_BUFFER - 1; i++){

      struct node_421 *newNode = (struct node_421*)
	malloc(sizeof(struct node_421));

      newNode->data = 0;
      newNode->next = NULL;
      ptr->next = newNode;
      ptr = ptr->next;
      
    }
    
    ptr->next = buff.write;
    tail = buff.write;
    head = buff.write;
    buff.read = head;

    printf("Buffer initialized \n");
    
    return 0;
    
  }
  
  else {
    printf("Failed to initialize buffer, buffer already exists \n");
    return -1;
  }
  
}

long insert_buffer_421(int i){

  if(head != NULL && buff.length < 20){
    
    buff.write->data = i;
    buff.length++;
    buff.write = buff.write->next;
    
    return 0;
  }

  else {
    printf("Failed to insert data into buffer, buffer does not exist " 
	   "or buffer is full \n");
    return -1;
  }
  
}

long print_buffer_421(void){

  if(head != NULL){

    int counter = 1;

    printf("\n");
    printf("Start of buffer print \n");
    
    do{

      printf("Node %d: %d\n", counter, buff.read->data);
      buff.read = buff.read->next;
      counter++;
      
    }while(buff.read != head);

    printf("End of buffer print \n");
    printf("\n");
    return 0;
  }

  else{
    printf("Failed to print buffer, buffer does not exist \n");  
    return -1;
  }
 
}

long delete_buffer_421(void){

  if(head != NULL){
    
    int counter = SIZE_OF_BUFFER;
    
    while(counter > 0){
      
      struct node_421 *temp = head->next;
      
      head->data = 0;
      free(head);
      head = temp;
      
      counter--;
    }
    
    head = NULL;
    buff.length = 0;
    
    printf("Buffer deleted \n");
    
    return 0;
  }

  else {
    printf("Failed to delete buffer, buffer does not exist \n");
    return -1;
  }

  
}
