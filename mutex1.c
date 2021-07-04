
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>


int	counter = 0;


void	*func(void *ptr)
{
	pthread_mutex_t *mutex;

	mutex = (pthread_mutex_t *) ptr; 
	pthread_mutex_lock(mutex);
	counter++;
	printf("Conter value ==> %d\n", counter);
	pthread_mutex_unlock(mutex);
	return ((void *) ptr);
}

int		main(void)
{
	pthread_mutex_t mutex;
	pthread_t thread1, thread2;
	int th1, th2;

	pthread_mutex_init(&mutex, NULL);
	th1 = pthread_create(&thread1,NULL, func, &mutex);
	th2 = pthread_create(&thread2,NULL, func, &mutex);
	if (th1 == 1)
		printf("Error ==> %d\n", th1);
	if (th2 == 1)
		printf("Error ==> %d\n", th2);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	return (0);
}
