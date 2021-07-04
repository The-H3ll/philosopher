
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int counter = 0;

void    *func(void *ptr)
{
/*    pthread_mutex_t *mutex;

    mutex = (pthread_mutex_t *) ptr;
    pthread_mutex_lock(mutex);*/
    printf("Thread number ==> %d\n", pthread_self());
    counter += 1;
   // pthread_mutex_unlock(mutex);
    return (ptr);
}

int main()
{
    int nmbr_threads = 10;
    pthread_mutex_t mutex;
    pthread_t threads[nmbr_threads];

    pthread_mutex_init(&mutex, NULL);
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < nmbr_threads; i++)
        {
            if (pthread_create(&threads[i], NULL, func, &mutex) == 1)
                printf("ERROR \n");
        }
    }
    for (int i = 0; i < nmbr_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("counter ==> %d\n", counter);
    return (0);
}
