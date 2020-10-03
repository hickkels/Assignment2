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
 
   // first create three queues, will return a pointer to the queue
   Queue *Q = CreateStringQueue(10); 
 
   // then create 4 pthreads using pthread_create
   // each has its own id
   pthread_t Reader;
   pthread_t Munch1; // use index!
   pthread_t Munch2; // use islower and toupper!
   pthread_t Writer; // when no more string to process, print # strings processed to stdout
  
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
