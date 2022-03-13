#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "list.h"
#include "schedulers.h"
#include "cpu.h"
#include "task.h"

void add(char *name, int priority, int burst);
void schedule();
void pickNextTask();
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
  
  printf("Round-Robin is an algorithm where each task in the list "
	 "is executed equally in "
	 "a specified time quantum \n");
  
  int remainingTime = 0;
  int counter = 0;
  
  while(head != NULL){

    remainingTime = 0;
    if(counter < 1){
      pickNextTask();
      counter++;
    }

    if(head->task->burst < QUANTUM){
      
      printf("The task [%s] from the list will be run with Round-Robin "
	     "with a burst of [%d] \n", head->task->name, head->task->burst);
      
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
    
    else if (head->task->burst > 0){

      // This allows the list to go back to the top
      
      struct node *temp = head;

      // Loops to the end of the list
      while(temp->next != NULL){
	temp = temp->next;
      }
      // Sets the end of the list to the head
      temp->next = head;
      
      temp = temp->next;

      // Continues iterating through the list
      head = head->next;

      temp->next = NULL;
    }
  }
  
  return;
}


void pickNextTask(){
  struct node *prev = NULL;
  struct node *current = head;
  struct node *next = NULL;

  while(current != NULL){

    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  head = prev;
}

void swap(struct node *a, struct node *b){
  struct task *temp = a->task;
  
  a->task = b->task;
  b->task = temp;
  return;
}

