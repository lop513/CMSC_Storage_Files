/*
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "schedulers.h"
#include "cpu.h"
#include "task.h"

void add(char *name, int priority, int burst);
void schedule();
void pickNextTask();

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

  // Runs the tasks
  while(head != NULL){
    run(head->task, head->task->burst);

    pickNextTask();
  }
  
  return;
}


void pickNextTask(){
  
  // Here we reverse the list so we can easily see first come first serve

  struct node* prev = NULL;
  struct node* current = head;
  struct node* next = NULL;

  while(current != NULL){

    next = current->next;

    current->next = prev;

    prev = current;
    current = next;
  }
  head = prev;

  delete(&head, head->task);
  
  return;
}
*/

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
  
  struct node *minNode = NULL;
  
  
  while(head != NULL){
    
    minNode = pickNextTask();
    swap(minNode, head);

    run(head->task, head->task->burst);

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
