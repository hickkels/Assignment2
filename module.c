#include "module.h"
#include <stdio.h>

//NOTES:
// should count empty lines: made up of two newline characters

/*
* Dynamically allocate a new Queue structure and initialize it with an array of character points of length size
* Returns a pointer to the new queue structure
*/
Queue *CreateStringQueue(int size){

    // malloc queue struct 
    Queue *q, q1;
    q = &q1; // not sure about this, leave it until testing
 
    // malloc the char ** array pointed to from that structure
    q->strings = malloc(sizeof(char*)*size);
    if ((q->strings)==NULL) {
        printf("Error while allocating 2D char array\n");
        exit(1);
    }     
   
    // sychronization vars used in the structure
    sem_init(q->OKToDequeue, 0); 
    sem_init(q->OKToEnqueue, 10);
    sem_init(q->MEQueue, 1);
    
    // state vars used in the structure
    q->curr_size = 0;

    return q;
}

/*
* Places the pointer to the string at the end of queue q
* If the queue is full, then this function blocks until there is space available
*/
void EnqueueString(Queue *q, char *string) {
    // SHOULD THIS start before or after the wait?
    time_t start_enqueue = time(NULL);
    sem_wait(q->OKToEnqueue);
    sem_wait(q->MEQueue);

    *(q->strings)[q->curr_size] = string;  // enqueue
    q->enqueueCount++;
    q->curr_size++;    

    sem_post(q->MEQueue);
    sem-post(q->OKToDequeue);
    // ???sentinel value to show the process is done enqueueing ? (don't count in queue stats)

    // stop recording enqueue time
    time_t end_enqueue = time(NULL);
    q->enqueueTime = ("%.2f\n", (long int) (end_enqueue - start_enqueue));

    return void;
}

/*
* Removes a pointer to a string from the beginning of queue q.
* If the queue is empty, then this function blocks until there is a string placed into the queue
* Returns the pointer that was removed from the queue
*/
char * DequeueString(Queue *q) {
    time_t start_dequeue = time(NULL);

    sem_wait(q->OKToDequeue);
    sem_wait(q->MEQueue);    

    // removes a pointer to a string from the beginning of queue q
    char *rem_string_ptr = *(q->strings)[q->curr_size];
    *(q->strings)[q->curr_size] = NULL; // ??is this right
    q->curr_size--;
    q->dequeueCount++;
   

    sem_post(q->MEQueue);
    sem-post(q->OKToEnqueue); 
    // stop recording dequeue time
    time_t end_dequeue = time(NULL);    
    q->dequeueTime = ("%.2f\n", (long int) (end_dequeue - start_dequeue));

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

