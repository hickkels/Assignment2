#include "module.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <semaphore.h> 
#include <sys/time.h>

const int QUEUE_SIZE = 10;
/*
* Dynamically allocate a new Queue structure and initialize it with an array of character points of length size
* Returns a pointer to the new queue structure
*/
Queue *CreateStringQueue(int size, int max_buff){

    // malloc queue struct 
    Queue *q =  malloc(sizeof(*q));
 
    // malloc the char ** array
    q->strings = malloc(sizeof(char*)*size);
    if (q->strings==NULL) {
        printf("Error while allocating 2D char array\n");
        exit(1);
    }
    // malloc each char *     
    for (int i=0; i<size; i++) {
        q->strings[i] = malloc(sizeof(char)*max_buff);
	if (q->strings[i]==NULL) {
	    printf("Error while allocating char pointer\n");
	    exit(1);
	}
    }  
 
    // sychronization vars used in the structure
    sem_init(&(q->OKToDequeue), 0, 0); 
    sem_init(&(q->OKToEnqueue), 0, 10);
    sem_init(&(q->MEQueue), 0, 1);
    
    // state vars used in the structure
    q->curr_size = 0;
    q->next_dq = 0;
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->first = 0;
    q->last = 0;
    
    return q;
}

/*
* Places the pointer to the string at the end of queue q
* If the queue is full, then this function blocks until there is space available
*/
void EnqueueString(Queue *q, char *string) {
    
    struct timeval startEnq, endEnq;  
    
    gettimeofday(&startEnq, 0);
    
    sem_check( sem_wait(&(q->OKToEnqueue)) );
    sem_check( sem_wait(&(q->MEQueue)) );

    // enqueue
    q->strings[q->last] = string;
    q->enqueueCount++;
    q->curr_size++;
    q->last = ModIncr(q->last);
    sem_check( sem_post(&(q->MEQueue)) ); 
    sem_check( sem_post(&(q->OKToDequeue)) );

    gettimeofday(&endEnq, 0);
    long secondsEnq = endEnq.tv_sec - startEnq.tv_sec;
    long microsecondsEnq = endEnq.tv_usec - startEnq.tv_usec;
    q->enqueueTime = secondsEnq + microsecondsEnq*1e-6;
}

/*
* Removes a pointer to a string from the beginning of queue q.
* If the queue is empty, then this function blocks until there is a string placed into the queue
* Returns the pointer that was removed from the queue
*/
char * DequeueString(Queue *q) {
    struct timeval startDeq, endDeq;

    gettimeofday(&startDeq, 0);

    sem_check( sem_wait(&(q->OKToDequeue)) );
    sem_check( sem_wait(&(q->MEQueue)) );    

    // removes the first char * from string to return
    char *rem_string_ptr = q->strings[q->first];
    q->strings[q->first] = NULL;
    q->next_dq++;
    q->dequeueCount++;
    q->first = ModIncr(q->first);
    sem_check( sem_post(&(q->MEQueue)) );
    sem_check( sem_post(&(q->OKToEnqueue)) ); 

    gettimeofday(&endDeq, 0);
    long secondsDeq = endDeq.tv_sec - startDeq.tv_sec;
    long microsecondsDeq = endDeq.tv_usec - startDeq.tv_usec;
    q->dequeueTime = secondsDeq + microsecondsDeq*1e-6;
    return rem_string_ptr;
}

int ModIncr(int v) {
     return (v+1)%QUEUE_SIZE;
}

void sem_check(int sem_return_val) {
    if(sem_return_val==-1) {
	printf("Semaphore error.\n");
	exit(1);
    }
}

/* 
* Prints the statistics for this queue
*/
void PrintQueueStats(Queue *q) {
    printf("Enqueue count: %d\n", q->enqueueCount);
    printf("Dequeue count: %d\n", q->dequeueCount);
    printf("Enqueue time: %.6f\n", q->enqueueTime);
    printf("Dequeue time: %.6f\n", q->dequeueTime);
}

