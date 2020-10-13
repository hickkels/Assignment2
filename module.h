#ifndef MODULE_HEADER
#define MODULE_HEADER
#include <semaphore.h>

typedef struct Queue
{
    int curr_size;
    int first;
    int last;
    int enqueueCount;
    int dequeueCount;
    int next_dq;
    sem_t OKToEnqueue;
    sem_t OKToDequeue;
    sem_t MEQueue;
    float enqueueTime;
    float dequeueTime;
    char **strings;
} Queue;

Queue *CreateStringQueue(int size, int max_buff);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
void sem_check(int sem_return_val);
int ModIncr(int v);

#endif
