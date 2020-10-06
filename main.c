/*
Cecelia Peterson: cpeterson36@wisc.edu, 9073157274
Kelsey Hickok: khickok@wisc.edu, 9076435016
*/

 /*
 The main function calls functions to print requested fields for given process ids
 Input: command line args
 Return: int representing exit on success or failure
 */
 int main(int argC, char *argV[]) {
  
    // create a synchronization var for each queue
   
 
    // first create three queues, will return a pointer to the queu
    Queue *Q = CreateStringQueue(10); 
 
    // each will be size
  
    // then create 4 pthreads using pthread_create
    // each has its own id
    pthread_t Reader;
    pthread_t Writer;
    pthread_t Munch1;
    pthread_t Munch2;
   
    int read = pthread_create(&Reader, NULL, *reader_function, NULL);
    int munch1 = pthread_create(&Munch1, NULL, *munch1_function, NULL);
    int munch2 = pthread_create(&Munch2, NULL, *munch2_function, NULL);
    int write = pthread_create(&Writer, NULL, *writer_function, NULL);
    
    if (!read) 
        pthread_join(Reader, NULL);
    if (!munch1)
        pthread_join(Munch1, NULL);
    if (!munch2)
        pthread_join(Munch2, NULL);
    if (!write)
        pthread_join(Writer, NULL);

    // thread Reader will read in each input line from stdin
    // must check that the input line does not exceed the size of your buffer
    // it it does: reject that line by 
    // (1) printing out an error message to stderr and 
    // (2) throw away (flush to end of line) any remaining characters on that line
   
    // wait for these threads to finish by calling pthread_join
    // each thread will call pthread_exit
  
    // threads should terminate when there is no more input (end of file) TRICKY
   
  
    // for each queue, print statistics to stderr uding PrintQueueStats function
  
  
   return 0;
 }

char* reader_function() {
    /* TODO:
    * pass each line to its own spot in the queue
    * change return
    */   
    char *buffer;
    size_t  buff_size = 1024;
    size_t characters;
    buffer = (char *)malloc(bufsize * sizeof(char));
    
    if( buffer == NULL) {
        fprintf(stderr, "Unable to allocate buffer.");
        exit(1);
    }
    
    characters = getline(&buffer, &buff_size, stdin);    

    if (characters > n) {
        fprintf(stderr, "Input line has exceeded buffer length.");
        for (int i=buff_size; i<characters; i++) {
            buffer[i] = '\0';
        }
    }
    return buffer;
}

char* munch1_function(char* buffer) {
    // use strchr
    /* TODO:
    * read in values in each spot in the queue
    * look for spaces in current string
    * change value to asterik
    * remove string
    * pass changed string to its own spot in the queue
    * continue to iterate through queue
    * change return
    */
    const char sp = ' ';
    const char ast = '*';
    int firstSpace; 
    firstSpace = strchr(buffer, sp);
    buffer[r] = ast;
    if (*r != NULL) {
        for (int i=r+1; i<buff_size; i++) {        
            r = strchr(buffer, sp);
            buffer[r] = ast;
        }
    }
    return buffer;   
}

char* munch2_function(char* buffer) {
    // use islower and toupper!
    /* TODO:
    * read in values in each spot in the queue
    * look for lowercase chars in current string
    * change value to uppercae
    * remove string
    * pass changed string to its own spot in the queue
    * continue to iterate through queue
    * change return
    */
    int lower;
    int upper;
    for (int i=0; i<buff_size; i++) {
        lower = islower(buffer[i]);
        if (lower > 0) {
            upper = toupper(buffer[i]);
            buffer[i] = upper;
        }
    } 
    return buffer;
}

void writer_function() {
    // when no more string to process, print # strings processed to stdout
    



}

