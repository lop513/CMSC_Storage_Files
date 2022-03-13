#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "list.h"
#include "schedulers.h"
#include "cpu.h"
#include "task.h"

void add(char *name, int priority, int burst);
void schedule();
struct node *pickNextTask(struct node *setHead);
void swap(struct node *a, struct node *b);

struct node *head;

void add(char *name, int priority, int burst){
  
  struct task *newTask = (struct task*) malloc(sizeof(struct task));
  
  newTask->name = name;
  newTask->priority = priority;
  newTask->burst = burst;

  insert(&head, newTask);
  printf("Inserted, [%s] [%d] [%d] \n",
	 head->task->name, head->task->priority, head->task->burst);
  
  return;
}


void schedule(){

  printf("Priority with Round-Robin is an algorithm where tasks are "
	 "executed in "
	 "order of their priority unless their priority is equal then they are "
	 "executed in a Round-Robin algorithm where each of those tasks are "
	 "run for a time quantum \n");
  
  struct node *priorityNode = NULL;
  struct node *checkNext = NULL;
  int remainingTime = 0;
  
  while(head != NULL){
    
    remainingTime = 0;
    
    priorityNode = pickNextTask(head);
    swap(priorityNode, head);
    
    checkNext = pickNextTask(head->next);
  
    
    if(head->next != NULL && head->task->priority == checkNext->task->priority){
     
      if(head->task->burst < QUANTUM){
	
	printf("The task [%s] from the list will be run with Round-Robin "
	     "with a burst of [%d] \n",
	       head->task->name, head->task->burst);
	

	run(head->task, head->task->burst);
      }
      
      else{
	
	printf("The task [%s] from the list will be run with Round-Robin "
	     "with a QUANTUM burst of [%d] \n", head->task->name, QUANTUM);
	

	run(head->task, QUANTUM);
      }
      // This is because this is a Round Robin algorithm, we have to subtract
      // the CPU's QUANTUM time from the remaining burst time
      remainingTime = head->task->burst - QUANTUM;
      head->task->burst = remainingTime;


      if(head->task->burst <= 0){
	printf("Remove [%s] from the list \n", head->task->name);
	delete(&head, head->task);
      
      }
    
      else if(head->task->burst >0){

	// This allows the list to go back to the top
      
	struct node *temp = head;
      
	while(temp->next != NULL){
	  temp = temp->next;
	}
	// Sets the end of the linked list to the head
	temp->next = head;

	temp = temp->next;

	// Continues iterating through the list
	head = head->next;

	temp->next = NULL;
      
      }
    }
    
    else {
      printf("The task with the highest priority from the list is [%s] "
	     "with a priority of [%d] and a burst of [%d] \n",
	     head->task->name, head->task->priority, head->task->burst);

      run(head->task, head->task->burst);
      
      printf("Remove [%s] from the list \n", head->task->name);
      delete(&head, head->task);
      
    }
    
  }
  
  return;
}


struct node *pickNextTask(struct node *setHead){
  struct node *tempHead = setHead;
  struct node *priorityNode = NULL;
  int priority  = INT_MIN;
  
  while(tempHead != NULL){
    
    if(priority < tempHead->task->priority){

      priority  = tempHead->task->priority;
      
      priorityNode = tempHead;
    }

    tempHead = tempHead->next;
  }
  
  return priorityNode;
}

void swap(struct node *a, struct node *b){
  struct task *temp = a->task;
  
  a->task = b->task;
  b->task = temp;
  return;
}
