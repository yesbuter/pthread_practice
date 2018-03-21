#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 115

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld\n", tid);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    long taskid[NUM_THREADS];

    for(t = 0; t < NUM_THREADS; t++)
    {
        taskid[t] = t;
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void*)taskid[t]);
        if(rc)
        {
            printf("ERROR; the %ldth return code from pthread_create is %d\n", t, rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
