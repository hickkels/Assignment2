#ifndef MODULE_HEADER
#define MODULE_HEADER
#include <semaphore.h>

typedef struct Queue
{
    int curr_size;
    int enqueueCount;
    int dequeueCount;
    sem_t OKToEnqueue;
    sem_t OKToDequeue;
    sem_t MEQueue;
    time_t enqueueTime;
    time_t dequeueTime;
    char **strings;
} Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);

#endif
