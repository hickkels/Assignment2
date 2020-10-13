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

// structs to define queues
struct Mult_args {
    Queue *arg1;
    Queue *arg2;
};

static size_t MAX_BUFF = 1024;
static int QUEUE_SIZE = 10;

/* Read from standard input, one line at a time. 
* Reader will take the each line of the input and pass it to thread Munch1 through a queue of character strings
*/
void* reader_function(void *queue_ptr) {
    
    Queue *reader_to_munch1 = (Queue *) queue_ptr; 
    size_t len = 0; // initial size of string
    char *string = NULL; // initial string set to NULL
    char ch; // character to be iterated and read in from stdin
    size_t characters = 0; // count of characters in string
    
    ch = (char) fgetc(stdin);
    while (EOF != ch) {
	string = malloc(MAX_BUFF * sizeof(char)); // allocate enough room for the string with the buffer size
        // while character iterated is not the end of line character
        while ((ch != '\n') && (characters<MAX_BUFF)) {
            string[len++] = ch; // read in characters and set equal to corresponding position
            characters++; // increment character count of string
            ch = (char) fgetc(stdin);
    	}
        // if input line exceeds buffer size
        if ((characters==MAX_BUFF) && (ch != '\n')) {
            fprintf(stderr, "Input line has exceeded buffer length.\n"); // print error
            // flush to end of live any remaining characters on that line
	        while ('\n' != ch) ch = (char) fgetc(stdin);
        } else {
            if (string != NULL)
                EnqueueString(reader_to_munch1, string); // pass line to queue
        }
    	ch = (char) fgetc(stdin);
        len = 0;
	characters = 0;
        string = NULL;
    }
    EnqueueString(reader_to_munch1, NULL);
    pthread_exit(0);
}

/* Scan the line and replace each space character (not tabs or newlines) with an asterisk ("*") character. 
* It will then pass the line to thread Munch2 through another queue of character strings.
*/
void* munch1_function(void *m1_args) {
    
    struct Mult_args *args = (struct Mult_args *)m1_args;
    Queue *reader_to_munch1 = (Queue *) args->arg1; 
    Queue *munch1_to_munch2 = (Queue *) args->arg2;

    char sp = ' ';
    char ast = '*';
    char *string;
    char *strPtr;

    // while the queue size is not exceeded
    while(1) {
	string = DequeueString(reader_to_munch1); // take out string and remove from queue
	strPtr = string; // set string pointer equal to string
        // while there is a space character found in the string
	if (NULL!=strPtr) {
	    while((strPtr = strchr (strPtr, sp)) != NULL) {
                *strPtr++ = ast; // replace space character with asterik 
            }
	}
        EnqueueString(munch1_to_munch2, string); // pass new string to queue
	if (NULL==string) break; //break out of loop if null end marker if reached
    }
    pthread_exit(0);
}

/* Scan the line and convert all lower case letters to upper case (e.g., convert "a" to "A"). 
* It will then pass the line to thread Writer though yet another queue of character strings.
*/
void* munch2_function(void *m2_args) {
    
    struct Mult_args *args = (struct Mult_args *) m2_args;

    // initialize queues used in munch2
    Queue *munch1_to_munch2 = (Queue *) (args->arg1);
    Queue *munch2_to_writer = (Queue *) (args->arg2);
    
    // initialize other variables used in munch2
    int lower = 0;
    int upper = 0;
    char *string;

    // while the queue size is not exceeded
    while (munch1_to_munch2->first <= munch1_to_munch2->last) {
	
	string = DequeueString(munch1_to_munch2); // take out string and remove from queue
        // iterate through string
	if (NULL!=string) {
            for (int i=0; (size_t)i<strlen(string); i++) {
                lower = islower(string[i]); // find lower case character
                // if lower case character = true
                if (lower > 0) {
                    upper = toupper(string[i]); // change character to uppercase
                    string[i] = upper; // update in ptr
                }
            }
	}
        EnqueueString(munch2_to_writer, string); // pass new string to queue
        if (NULL==string) break; //break out of loop if null end marker if reached
    }
    pthread_exit(0);
}

/* Write line to standard output */
void* writer_function(void *queue_ptr) {

    Queue *munch2_to_writer = (Queue *) queue_ptr;
    char *outString;
    
    // while the queue size is not exceeded
    while (1) {
        outString = DequeueString(munch2_to_writer); // take out string and remove from queue
    if (NULL==outString) break; //break out of loop if null end marker if reached
        printf("%s\n", outString); // print string
        free(outString);
    }
    pthread_exit(0);
}

 /*
 The main function calls functions to print requested fields for given process ids
 Input: command line args
 Return: int representing exit on success or failure
 */
 int main() {
    
    // first create three queues, will return a pointer to the queue
    Queue *reader_to_munch1 = CreateStringQueue(QUEUE_SIZE, MAX_BUFF); 
    Queue *munch1_to_munch2 = CreateStringQueue(QUEUE_SIZE, MAX_BUFF);
    Queue *munch2_to_writer = CreateStringQueue(QUEUE_SIZE, MAX_BUFF);
  
    // then create 4 pthreads using pthread_create, each has its own id
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
    int read = pthread_create(&Reader, NULL, &reader_function, (void *)(reader_to_munch1));
    int munch1 = pthread_create(&Munch1, NULL, &munch1_function, (void *)(&m1_args));
    int munch2 = pthread_create(&Munch2, NULL, &munch2_function, (void *)(&m2_args));
    int write = pthread_create(&Writer, NULL, &writer_function, (void *)(munch2_to_writer));
   
    // wait for these threads to finish by calling pthread_join
    if (!read) pthread_join(Reader, NULL);
    if (!munch1) pthread_join(Munch1, NULL);
    if (!munch2) pthread_join(Munch2, NULL);
    if (!write) pthread_join(Writer, NULL);
 
    // for each queue, print statistics to stderr uding PrintQueueStats function
    printf("Queue statistics: \n");
    PrintQueueStats(munch2_to_writer);
    pthread_exit(0);
}

