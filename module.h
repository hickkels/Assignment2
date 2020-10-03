#ifndef MODULE_HEADER
#define MODULE_HEADER

Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
typedef struct Queue
{
    int sizeQueue;
    int currSize;
    int enqueCount;
    int dequeue;
    time_t enqueueTime;
    time_t dequeueTime;
    char *strings;
}Queue;

#endif
