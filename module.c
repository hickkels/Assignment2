#include "module.h"
#include <stdio.h>

int enqueueCount;
int dequeueCount;
wallclock enqueueTime
wallclock dequeueTime;

/*
* Dynamically allocate a new Queue structure and initialize it with an array of character points of length size
* Returns a pointer to the new queue structure
*/
Queue *CreateStringQueue(int size){

     // malloc queue struct

      

}

/*
* Places the pointer to the string at the end of queue q
* If the queue is full, then this function blocks until there is space available
*/
void EnqueueString(Queue *q, char *string) {
   
    if (q->size == q->curr_size) //THE BLOCK until space availible
    &(q->strings)[q->curr_size] = string;

}



/*
* Removes a pointer to a string from the beginning of queue q.
* If the queue is empty, then this function blocks until there is a string placed into the queue
* Returns the pointer that was removed from the queue
*/
char * DequeueString(Queue *q) {




}



/* 
* Prints the statistics for this queue
*/
void PrintQueueStats(Queue *q) {







}

