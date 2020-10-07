/*
Cecelia Peterson: cpeterson36@wisc.edu, 9073157274
Kelsey Hickok: khickok@wisc.edu, 9076435016
*/

#include <stdio.h>
#include <string.h>
#include "module.h"
#include <stdlib.h>
#include <ctype.h>

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
   
    // then create 4 pthreads using pthread_create 
    int read = pthread_create(&Reader, NULL, reader_function, NULL);
    int munch1 = pthread_create(&Munch1, NULL, munch1_function, NULL);
    int munch2 = pthread_create(&Munch2, NULL, munch2_function, NULL);
    int write = pthread_create(&Writer, NULL, writer_function, NULL);
    
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
  
   return 0;
 }

void reader_function() {
    /* TODO:
    * pass each line to its own spot in the queue
    * change return
    */   
    char input[1024];
    char *buffer;
    size_t  buff_size = 1024;
    size_t characters;
    buffer = (char *)malloc(buff_size * sizeof(char));

    // read in input file from stdin and set to input
    while(*(fgets(input, 1024, stdin)) != EOF) {
        // get the current line

    // thread Reader will read in each input line from stdin
    // must check that the input line does not exceed the size of your buffer
    // it it does: reject that line by
    // (1) printing out an error message to stderr and
    // (2) throw away (flush to end of line) any remaining characters on that line
        if(buffer == NULL) {
            fprintf(stderr, "Unable to allocate buffer.");
            exit(1);
        }
        characters = getline(&buffer, &buff_size, stdin);    
        if (characters > buff_size) {
            fprintf(stderr, "Input line has exceeded buffer length.");
            for (int i=buff_size; i<characters; i++) {
                buffer[i] = '\0';
            }
        }
        reader_to_munch1.EnqueueString(input);
        // increment line ??
    }
}

void munch1_function() {
    // use strchr
    /* TODO:
    * change value to asterik (use correct memory addressing)
    */
    const char sp = ' ';    
    const char ast = '*';
    int *firstSpace;
    char *string;

    string = reader_to_munch1.DequeueString();
    firstSpace = &strchr(string, sp);
    string[*firstSpace] = ast;
    if (firstSpace != NULL) {
        for (int i=*firstSpace+1; i<strlen(string); i++) {        
            firstSpace = &strchr(string, sp);
            string[*firstSpace] = ast;
        }
    }
    munch1_to_munch2.EnqueueString(string);
}

void munch2_function() {
    // use islower and toupper!
    /* TODO:
    * use correct memory addressing
    */
    int lower;
    int upper;
    char *string;
    string = munch1_to_munch2.DequeueString();
    for (int i=0; i<strlen(string); i++) {
        lower = islower(string[i]);
        if (lower > 0) {
            upper = toupper(string[i]);
            string[i] = upper;
        }
    }
    munch2_to_writer.EnqueueString(string); 
}

void writer_function() {
    // when no more string to process, print # strings processed to stdout
    



}

