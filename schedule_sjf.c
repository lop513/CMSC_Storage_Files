#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "list.h"
#include "schedulers.h"
#include "cpu.h"
#include "task.h"

void add(char *name, int priority, int burst);
void schedule();
struct node *pickNextTask();
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

  printf("The Shortest Job First algorithm is an algorithm "
	 "in which the tasks are executed with the "
	 "least amount of burst time first and the "
	 "most amount of burst time last \n");
  
  struct node *minNode = NULL;
  
  
  while(head != NULL){
    
    minNode = pickNextTask();
    swap(minNode, head);
    
    printf("The task with the shortest burst time from the list is [%s] "
	   "with a burst of [%d] \n",head->task->name, head->task->burst);
    
    run(head->task, head->task->burst);

    printf("Remove [%s] from the list \n", head->task->name);
    
    delete(&head, head->task);
  }
  
  return;
}


struct node *pickNextTask(){
  struct node *tempHead = head;
  struct node *minNode = NULL;
  int min = INT_MAX;
  
  while(tempHead != NULL){

    if(min > tempHead->task->burst){

      min = tempHead->task->burst;
      
      minNode = tempHead;
    }

    tempHead = tempHead->next;
  }
  
  return minNode;
}

void swap(struct node *a, struct node *b){
  struct task *temp = a->task;
  
  a->task = b->task;
  b->task = temp;
  return;
}
