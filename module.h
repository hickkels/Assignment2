/*
Cecelia Peterson: cpeterson36@wisc.edu, 9073157274
Kelsey Hickok: khickok@wisc.edu, 9076435016
*/
#ifndef MODULE_HEADER
#define MODULE_HEADER
#include <semaphore.h>

typedef struct Queue
{
    int curr_size;
    int enqueueCount;
    int dequeueCount;
    int next_dq;
    int size;
    sem_t OKToEnqueue;
    sem_t OKToDequeue;
    sem_t MEQueue;
    double enqueueTime;
    double dequeueTime;
    char **strings;
} Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
void sem_check(int sem_return_val);

#endif
