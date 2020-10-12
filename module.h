#ifndef MODULE_HEADER
#define MODULE_HEADER
#include <semaphore.h>

typedef struct Queue
{
    int curr_size;
    sem_t OKToEnqueue;
    sem_t OKToDequeue;
    sem_t MEQueue;
    int enqueueCount;
    int dequeueCount;
    time_t enqueueTime;
    time_t dequeueTime;
    char **strings;
} Queue;

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);

#endif
