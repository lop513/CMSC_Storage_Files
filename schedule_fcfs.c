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

  printf("The First Come First Serve algorithm is an "
	 "algorithm in which the first task inserted in the "
	 "list is the first task executed and the last task "
	 "inserted in the list is the last task executed \n");
  
  struct node *minNode = NULL;
  
  
  while(head != NULL){
    
    minNode = pickNextTask();
    swap(minNode, head);

      printf("The next task in the list is [%s] "
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
  //char taskID = tempHead->task->name[1];
  int min  = INT_MAX;
  
  while(tempHead != NULL){

    int taskID = (int)(tempHead->task->name[1]);
    //printf("%d", taskID);
    
    if(min > taskID){

      min  = (int)(tempHead->task->name[1]);
      
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
