/**
 * @author MARK GARCIA
 *         mark.garcia01@student.csulb.edu
 *
 * Project 1 CECS 326 -- Multithreaded Programming and Synchonization
 *
 * PART 2 - With Synchronization
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int sharedVar = 0;

//create mutex and barrier
pthread_mutex_t mut;
pthread_barrier_t bar;

void SimpleThread(int);

// Function given in program instructions.
void *SimpleThead(void *which){
    int num, val;
    int threadID = (int *) which;

    for (num = 0; num < 20; num++){
#ifdef PTHREAD_SYNC
pthread_mutex_lock(&mut); // lock the thread so that only one task is being worked on.
#endif
        val = sharedVar;
        printf("***thread %d sees value %d\n", threadID, val);
        sharedVar = val + 1;

#ifdef PTHREAD_SYNC
pthread_mutex_unlock(&mut); // unlock thread so other threads can be accessible
#endif
    }

    pthread_barrier_wait(&bar);
    val = sharedVar;
    printf("Thread %d sees final value %d\n", threadID, val);
}

/*void * entry_point(int *arg){
    int *which_ptr = arg[0];
    int which = &which_ptr;
    SimpleThead(which);
    return NULL;
}*/

int main(int argc, char * argv[]){

    //Start by checking command line args are valid.
    if (argc != 2){
        printf("Enter a valid number of arguements.");
        return 0;
    }
    //valid num of args entered, count num of threads from arg

    int numThreads = atoi(argv[1]);

    //check if the arg is a valid value... ie a number greater 1 since having the arg
    //set to 0 makes no sense.

    if (numThreads < 1){
        printf("Enter a number greater than 1.");
        return 0;
    }

    //initialize mutex and barrier
    pthread_mutex_init(&mut, NULL);
    pthread_barrier_init(&bar, NULL, numThreads);

    //Everything is checked. Start creating threads and pass use SimpleThread function.

    pthread_t threads[numThreads]; //array to keep track of threads. where we will join the threads

    for (int i = 0; i < numThreads; i++) {
        //create the threads.
        pthread_create(&(threads[i]), NULL, SimpleThead, (void *) i);
    }

    for (int i = 0; i < numThreads; i++){
        pthread_join(threads[i],NULL);
    }




    return 0;
}
