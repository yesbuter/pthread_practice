#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 50

volatile int count = 0;

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *PrintHello(void *threadid)
{
    printf("tread %ld is starting\n", pthread_self());
    fflush(stdout);
    int rc = pthread_mutex_lock(&mutex);
    if(rc != 0)
        printf("error to lock mutex, rc is %d\n", rc);
    printf("start wait , count: %d\n", count);
    
    count++;
    
    if(count != 50)
    {
        rc = pthread_cond_wait(&cond, &mutex);
    }
    else{
        pthread_cond_broadcast(&cond);
        printf("have sent broadcast\n");
    }
        long tid;
        tid = (long)threadid;
        printf("Hello World! It's me, thread #%ld\n", tid);
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    long taskid[NUM_THREADS];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t = 0; t < NUM_THREADS; t++)
    {
        taskid[t] = t;
        printf("In main: creating thread %ld\n", t);
        fflush(stdout);
        rc = pthread_create(&threads[t], &attr, PrintHello, (void*)taskid[t]);
        if(rc)
        {
            printf("ERROR; the %ldth return code from pthread_create is %d\n", t, rc);
            exit(-1);
        }
    }
    
    
    printf("start join\n");
    for(t = 0; t < NUM_THREADS; t++)
    {
        pthread_join(threads[t], NULL);
        printf("joined %ld\n", t);
        fflush(stdout);
    }
    printf("join ok\n");
    fflush(stdout);

    pthread_attr_destroy(&attr);
    printf("main thread id: %ld is closed\n", pthread_self());
    fflush(stdout);
    pthread_exit(NULL);
}
