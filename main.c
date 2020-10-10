/*
Cecelia Peterson: cpeterson36@wisc.edu, 9073157274
Kelsey Hickok: khickok@wisc.edu, 9076435016
*/

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "module.h"
#include <stdlib.h>

struct Mult_args {
    Queue *arg1;
    Queue *arg2;
};

void* reader_function(void *queue_ptr) {
    Queue *reader_to_munch1 = (Queue *) queue_ptr; 
    size_t  buff_size = 1024;
    size_t len = 0;
    char* string = NULL;
    int ch;
    int characters = 0;

    // thread Reader will read in each input line from stdin
    // must check that the input line does not exceed the size of your buffer
    // it it does: reject that line by
    // (1) printing out an error message to stderr and
    // (2) throw away (flush to end of line) any remaining characters on that line
    
    /* TODO:
    * change usage of c and fgetc (int vs char)
    */
    while (EOF != (ch = fgetc(stdin))) {
        while ((ch = fgetc(stdin)) != '\n') {
            string = malloc(buff_size * sizeof(char));
            string[len++] = ch;
            characters++;
        }
        if (characters > buff_size) {
            fprintf(stderr, "Input line has exceeded buffer length.");
            for (int i=buff_size; i<characters; i++) {
                string[i] = '\0';
            }
        } else {
            EnqueueString(reader_to_munch1, string);
        } 
    }
    EnqueueString(reader_to_munch1, NULL);
    pthread_exit(0);
}

void* munch1_function(void *m1_args) {
    // use strchr
    struct Mult_args *args = (struct Mult_args *)m1_args;
    Queue *reader_to_munch1 = (Queue *) args->arg1; 
    Queue *munch1_to_munch2 = (Queue *) args->arg2;
    char sp = ' ';
    char ast = '*';
    char* string;
    char* strPtr;
    int count = 0;
    while(reader_to_munch1->curr_size >= count) {
        string = DequeueString(reader_to_munch1);
        strPtr = string;
        while((strPtr = strchr (strPtr, sp)) != NULL) {
            *strPtr++ = ast;
        }
        EnqueueString(munch1_to_munch2, string);
        count++;
    }    
    pthread_exit(0);
}

void* munch2_function(void *m2_args) {
    // use islower and toupper!
    /* TODO:
    * use correct memory addressing
    */
    struct Mult_args *args = (struct Mult_args *)m2_args;
    Queue *munch1_to_munch2 = (Queue *) (args->arg1);
    Queue *munch2_to_writer = (Queue *) (args->arg2);
    int lower;
    int upper;
    char *string;
    int count = 0;

    while (munch1_to_munch2->curr_size >= count) {
        string = DequeueString(munch1_to_munch2);
        for (int i=0; i<strlen(string); i++) {
            lower = islower(string[i]);
            if (lower > 0) {
                upper = toupper(string[i]);
                string[i] = upper;
            }
        }
        EnqueueString(munch2_to_writer, string);
        count++;
    }
    pthread_exit(0);
}

void* writer_function(void *queue_ptr) {
    // when no more string to process, print queue statistics
    // print each string

    Queue *munch2_to_writer = (Queue *) queue_ptr;
    char *outString;
    int count = 0;

    printf("Output: \n");
    // while the counter value isn't greater than the last string's address space
    while (munch2_to_writer->curr_size >= count)
    {
        outString = DequeueString(munch2_to_writer);
        printf("%s\n", outString);
        count++;
    }
    pthread_exit(0);
}

 /*
 The main function calls functions to print requested fields for given process ids
 Input: command line args
 Return: int representing exit on success or failure
 */
 int main(int argC, char *argV[]) {
    
    printf("Please enter 10 lines:\n");
    // first create three queues, will return a pointer to the queue
    // make queue size 10 for testing purposes
    Queue *reader_to_munch1 = CreateStringQueue(10); 
    Queue *munch1_to_munch2 = CreateStringQueue(10);
    Queue *munch2_to_writer = CreateStringQueue(10);
  
    // then create 4 pthreads using pthread_create
    // each has its own id
    pthread_t Reader;
    pthread_t Writer;
    pthread_t Munch1;
    pthread_t Munch2;

    struct Mult_args m1_args;
    m1_args.arg1 = reader_to_munch1;
    m1_args.arg2 = munch1_to_munch2;

    struct Mult_args m2_args;
    m2_args.arg1 = munch1_to_munch2;
    m2_args.arg2 = munch2_to_writer;
    
    // then create 4 pthreads using pthread_create 
    int read = -pthread_create(&Reader, NULL, &reader_function, (void *)(reader_to_munch1));
    int munch1 = -pthread_create(&Munch1, NULL, &munch1_function, (void *)(&m1_args));
    int munch2 = -pthread_create(&Munch2, NULL, &munch2_function, (void *)(&m2_args));
    int write = -pthread_create(&Writer, NULL, &writer_function, (void *)(munch2_to_writer));
    
    // wait for these threads to finish by calling pthread_join
    if (!read) 
        pthread_join(Reader, NULL);
    if (!munch1)
        pthread_join(Munch1, NULL);
    if (!munch2)
        pthread_join(Munch2, NULL);
    if (!write)
        pthread_join(Writer, NULL);

    // each thread will call pthread_exit
  
    // threads should terminate when there is no more input (end of file) TRICKY 
  
    // for each queue, print statistics to stderr uding PrintQueueStats function
  
    printf("Queue statistics: \n");
    PrintQueueStats(munch2_to_writer);
    pthread_exit(0);
}

