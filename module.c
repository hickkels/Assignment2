#include "module.h"
#include <stdio.h>

int enqueueCount;
int dequeueCount;
time_t enqueueTime;  // amount of elapsed time a thread spent trying to enqueue
time_t dequeueTime;  // amount of elapsed time a thread spent trying to dequeue
sem_t OKToDequeue;
sem_t OKToEnqueue;
sem_t Mutex;
volatile int ...

/*
* Dynamically allocate a new Queue structure and initialize it with an array of character points of length size
* Returns a pointer to the new queue structure
*/
Queue *CreateStringQueue(int size){

    // malloc queue struct 
    Queue *q, q1;
    q = &q1; // not sure about this, leave it until testing
 
    // malloc the char ** array pointed to from that structure
    q->strings = malloc(sizeof(char*)*10);
    if ((q->strings)==NULL) {
        printf("Error while allocating 2D char array\n");
        exit(1);
    }     
   
    // sychronization vars used in the structure
    sem_init(q->OKToDequeue, 1); 
    sem_init(q->OKToEnqueue, 1);
    sem_init(q->Mutex, 1);
    
    // state vars used in the structure
    // do we need any?      

    // return a pointer to a new queue structure
    return q;
}

/*
* Places the pointer to the string at the end of queue q
* If the queue is full, then this function blocks until there is space available
*/
void EnqueueString(Queue *q, char *string) {

    // start recording enqueue time
    time_t start_enqueue = time(NULL);
     
    sem_wait(q->OKToEnqueue);
    q->enqueueCount = q->enqueueCount + 1;
    
    if (q->size == q->curr_size) //THE BLOCK until space availible
    &(q->strings)[q->curr_size] = string;  //does this need to be malloc-ed ?
    // MALLOC
    // sentinel value to show the process is done enqueueing ? (don't count in queue stats)

    // stop recording enqueue time
    time_t end_enqueue = time(NULL);
    
    // take the difference of these times for enqueue time
    q->enqueueTime = ("%.2f\n", (long int) (end_enqueue - start_enqueue));

    return void;
}

/*
* Removes a pointer to a string from the beginning of queue q.
* If the queue is empty, then this function blocks until there is a string placed into the queue
* Returns the pointer that was removed from the queue
*/
char * DequeueString(Queue *q) {

    // start recording dequeue time
    time_t start_dequeue = time(NULL);

    // if queue is empty, blocks until there is a string placed into the queue
    sem_wait(q->OKToDequeue);
    q->dequeueCount = q->dequeueCount + 1;

    // removes a pointer to a string from the beginning of queue q
    char *rem_string_ptr;
    // FREE

    // stop recording dequeue time
    time_t end_dequeue = time(NULL);    

    // take the difference of these times for dequeue time
    q->dequeueTime = ("%.2f\n", (long int) (end_dequeue - start_dequeue));

    // return pointer that was removed from the queue
    return rem_string_ptr;
}

/* 
* Prints the statistics for this queue
*/
void PrintQueueStats(Queue *q) {
    fprintf("Enqueue count: %d\n", q->enqueueCount);
    fprintf("Dequeue count: %d\n", q->dequeueCount);
    fprintf("Enqueue time: %.2f\n", q->enqueueTime);
    fprintf("Dequeue time: %.2f\n", q->dequeueTime);
    return void;
}

