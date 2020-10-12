#define _GNU_SOURCE
#include "module.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <semaphore.h> 

//NOTES:
// should count empty lines: made up of two newline characters

/*
* Dynamically allocate a new Queue structure and initialize it with an array of character points of length size
* Returns a pointer to the new queue structure
*/
Queue *CreateStringQueue(int size){

    printf("In create string queue\n");
    // malloc queue struct 
    Queue *q;
    q = malloc(sizeof(Queue));
 
    // malloc the char ** array pointed to from that structure
    q->strings = malloc(sizeof(char*)*size);
    if ((q->strings)==NULL) {
        printf("Error while allocating 2D char array\n");
        exit(1);
    }
    // malloc each char *     
    for (int i=0; i<size; i++) {
        q->strings[i] = malloc(sizeof(char)*4096);
    }  
 
    // sychronization vars used in the structure
    sem_init(&(q->OKToDequeue), 0, 0); 
    sem_init(&(q->OKToEnqueue), 0, 10);
    sem_init(&(q->MEQueue), 0, 1);
    
    // state vars used in the structure
    q->curr_size = 0;

    return q;
}

/*
* Places the pointer to the string at the end of queue q
* If the queue is full, then this function blocks until there is space available
*/
void EnqueueString(Queue *q, char *string) {
   
    printf("ENTERED ENQUEUE\n");
    // SHOULD THIS start before or after the wait?
    time_t start_enqueue = time(NULL);

    printf("after time\n");  
  
    int sem_check; 
    sem_check = sem_wait(&(q->OKToEnqueue));
    printf("a\n");
    if (sem_check==-1) {
        printf("Error waiting to OKToEnqueue\n");
    }
    printf("before MEQ\n");
    sem_check = sem_wait(&(q->MEQueue));
    printf("after MEQ\n");
    if (sem_check==-1) {
	printf("Error waiting to MEQueue\n");
    }

    // enqueue
    q->strings[q->curr_size] = string;
    q->enqueueCount++;
    q->curr_size++;    

    sem_check = sem_post(&(q->MEQueue));
    if (sem_check==-1) {
        printf("Error posting to MEQueue\n");
    }
    sem_check = sem_post(&(q->OKToDequeue));
    if (sem_check==-1) {
	printf("Error posting to OKToDequeue\n");
    }

    printf("STRING ADDED: %s\n", string);

    time_t end_enqueue = time(NULL);
    q->enqueueTime = (long int) (end_enqueue - start_enqueue);
}

/*
* Removes a pointer to a string from the beginning of queue q.
* If the queue is empty, then this function blocks until there is a string placed into the queue
* Returns the pointer that was removed from the queue
*/
char * DequeueString(Queue *q) {
    printf("ENTERED DEQUEUE\n");
    time_t start_dequeue = time(NULL);

    int sem_check;
    sem_check = sem_wait(&(q->OKToDequeue));
    if (sem_check==-1) {
        printf("Error waiting to OKToDequeue\n");
    }
    sem_check = sem_wait(&(q->MEQueue));    
    if (sem_check==-1) {
        printf("Error waiting to MEQueue\n");
    }

    // removes a pointer to a string from the beginning of queue q
    char *rem_string_ptr = *((q->strings)+(q->curr_size));
    *((q->strings)+(q->curr_size)) = NULL;
    q->curr_size--;
    q->dequeueCount++;
    printf("Dequeued a string\n");

    sem_check = sem_post(&(q->MEQueue));
    if (sem_check==-1) {
        printf("Error posting to MEQueue\n");
    }
    sem_check = sem_post(&(q->OKToEnqueue)); 
    if (sem_check==-1) {
        printf("Error posting to OKToEnqueue\n");
    }

    time_t end_dequeue = time(NULL);    
    q->dequeueTime = (long int) (end_dequeue - start_dequeue);

    return rem_string_ptr;
}

/* 
* Prints the statistics for this queue
*/
void PrintQueueStats(Queue *q) {
    printf("Enqueue count: %d\n", q->enqueueCount);
    printf("Dequeue count: %d\n", q->dequeueCount);
    printf("Enqueue time: %.2ld\n", q->enqueueTime);
    printf("Dequeue time: %.2ld\n", q->dequeueTime);
}

