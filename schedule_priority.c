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
  
  return;
}


void schedule(){
  
  struct node *priorityNode = NULL;
  
  
  while(head != NULL){
    
    priorityNode = pickNextTask();
    swap(priorityNode, head);

    run(head->task, head->task->burst);

    delete(&head, head->task);
  }
  
  return;
}


struct node *pickNextTask(){
  struct node *tempHead = head;
  struct node *priorityNode = NULL;
  int priority  = INT_MIN;
  
  while(tempHead != NULL){

    if(priority < tempHead->task->priority){

      priority = tempHead->task->priority;
      
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
