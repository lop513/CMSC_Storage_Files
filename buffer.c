// This file is the implementation of the buffer in kernel space

#include "buffer.h"
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

static struct ring_buffer_421 buff;
struct node_421 *head;
struct node_421 *tail;
int init_check = 0;

SYSCALL_DEFINE0(init_buffer_421){
  
  struct node_421 *ptr;
  struct node_421 *newNode;
  int i;

  
  if(init_check == 0){


    head = kmalloc(sizeof(struct node_421), GFP_KERNEL);
    buff.write = head;
    buff.write->data = 0;   
    
    ptr = buff.write;
    for(i = 0; i < SIZE_OF_BUFFER - 1; i++){

      newNode = kmalloc(sizeof(struct node_421), GFP_KERNEL);

      newNode->data = 0;
      ptr->next = newNode;
      ptr = ptr->next;
      
    }
    
    ptr->next = buff.write;
    tail = buff.write;
    head = buff.write;
    buff.read = head;
    buff.length = 0;
    
    printk("Buffer initialized \n");
    init_check = 1;
    return 0;
    
  }
  
  else {
    printk("Failed to initialize buffer, buffer already exists \n");
    return -1;
  }
  
}

SYSCALL_DEFINE1(insert_buffer_421, int, i){

  if(init_check == 1) {
    if(buff.length < 20){
    
      buff.write->data = i;
      buff.length++;
      buff.write = buff.write->next;
     
      return 0;
    }
    printk("Failed to insert data into buffer, buffer is full \n");
    return -1;
  }

  else {
    printk("Failed to insert data into buffer, buffer does not exist \n");
    return -1;
  }
  
}

SYSCALL_DEFINE0(print_buffer_421){
  int counter;
  if(init_check == 1){

    counter = 1;

    printk("\n");
    printk("Start of buffer print \n");

    do{

      printk("Node %d: %d\n", counter, buff.read->data);
      buff.read = buff.read->next;
      counter++;
      
    }while(buff.read != head);

    printk("End of buffer print \n");
    printk("\n");
    return 0;
  }

  else{
    printk("Failed to print buffer, buffer does not exist \n");  
    return -1;
  }
 
}

SYSCALL_DEFINE0(delete_buffer_421){
  int counter;
  struct node_421 *temp;
  
  if(init_check == 1){
    
    counter = SIZE_OF_BUFFER;
    
    while(counter > 0){
      
      temp = head->next;
      
      head->data = 0;
      kfree(head);
      head = temp;
      
      counter--;
    }
    
    
    buff.length = 0;
    
    printk("Buffer deleted \n");
    init_check = 0;
    return 0;
  }

  else {
    printk("Failed to delete buffer, buffer does not exist \n");
    return -1;
  }

}


