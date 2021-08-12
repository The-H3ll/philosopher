/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molabhai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:38:20 by molabhai          #+#    #+#             */
/*   Updated: 2021/06/29 18:38:21 by molabhai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	checking(t_thread *thread)
{
	int i;

	i = 0;
	while (i < thread->philo_nmbr)
	{
	//	printf("THREAD ==> %d\n", check[i]);
		if (check[i] == 0)
			return ;
		i += 1;
	}
	checkout = 1;
	while (1);
}

void	check_death(int life,  t_thread *thread)
{
	struct timeval  current;
	int current_time;
	//pthread_mutex_lock(&thread->mutex[thread->id]);
	gettimeofday(&current, NULL);
	current_time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
	if (checkout == 1)
		while (1);
	if ((current_time - life) > thread->to_die && checkout == 0)
	{
//		printf("time ==> %d || Death ==> %d\n", (current_time -life), thread->to_die);
		printf("\033[0;31m");
		printf("%d %d died\n", current_time, thread->id);
		printf("\033[0m");
		checkout = 1;
		while (1);
	}
	//pthread_mutex_unlock(&thread->mutex[thread->id]);
}

void	*checkout_(void *ptr)
{
	while (1)
	{
		if (checkout == 1)
			break ;
	}
	return (ptr);
}

void    *func(void *ptr)
{
	t_thread *thread;
	struct timeval current;
	int current_time;
	
	thread = (t_thread *) ptr;	
	memset(check, 0, sizeof(int));
	gettimeofday(&current, NULL);
	thread->life = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
	if (thread->philo_nmbr < 2 || thread->time == 0)
	{
		printf("\033[0;31m");
		printf("%d %d died\n", thread->life, thread->id);
		printf("\033[0m");
		checkout = 1;
	}
	if ((thread->id % 2) == 0)
	{
		if (thread->to_die < thread->to_eat)
			usleep(thread->to_die * 1000);
		else
			usleep(thread->to_eat * 1000);
	}
	check_death(thread->life, thread);
	while (1)
	{
		// Eating
		if (thread->id >= 0 && thread->id < thread->philo_nmbr)
		{
			if (pthread_mutex_lock(&thread->mutex[thread->id]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
			if (pthread_mutex_lock(&thread->mutex[thread->id - 1]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
		}
		else if (thread->id == thread->philo_nmbr)
		{
			if (pthread_mutex_lock(&thread->mutex[0]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
			if (pthread_mutex_lock(&thread->mutex[thread->id - 1]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
		}
		gettimeofday(&current, NULL);
		current_time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
		printf("\033[0;32m");
		printf("%d %d is eating\n", current_time, thread->id);
		printf("\033[0m");
		usleep(thread->to_eat * 900);
		check_death(thread->life,  thread);
		gettimeofday(&current, NULL);
		thread->life = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
		if (thread->time > 0)
			thread->time -= 1;
		if (thread->time == 0)
			check[thread->id - 1] = 1;
//		printf("Thread time ==> %d [|| threadId ==> %d\n", check[thread->id - 1], thread->id);
		checking(thread);
		if (thread->id >= 0 && thread->id < thread->philo_nmbr)
		{
			if (pthread_mutex_unlock(&thread->mutex[thread->id]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
			if (pthread_mutex_unlock(&thread->mutex[thread->id - 1]) != 0)
				printf("Error in mutex lock %d\n", thread->id);
		}
		else if (thread->id == thread->philo_nmbr)
		{
			if (pthread_mutex_unlock(&thread->mutex[0]) != 0)
				printf("Error in mutex unlock %d\n", thread->id);
			if (pthread_mutex_unlock(&thread->mutex[thread->id - 1]) != 0)
				printf("Error in mutex unlock %d\n", thread->id);
		}
		// Sleeping
		gettimeofday(&current, NULL);
		current_time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
		printf("\033[0;33m");
		printf("%d %d is sleeping\n", current_time, thread->id);
		printf("\033[0m");
		usleep(thread->to_sleep * 900);
		check_death(thread->life, thread);
		// Thinking
		gettimeofday(&current, NULL);
		current_time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
		printf("%d %d is thinking\n", current_time, thread->id);
	}
}

void    philosopher(t_philo philo)
{
	int i;
	t_thread **thread;
	pthread_t pt;
	pthread_mutex_t *mutex;

	i = 0;
	check = malloc(philo.philo * sizeof(int)); 
	mutex = malloc(sizeof(pthread_mutex_t) * philo.philo);
	thread = malloc(sizeof(t_thread *) * philo.philo);
	while (i < philo.philo)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	i = 0;
	while (i < philo.philo)
	{
			thread[i] = malloc(sizeof(t_thread));
			thread[i]->id = i + 1;
			thread[i]->mutex = mutex;
			thread[i]->philo_nmbr = philo.philo;
			thread[i]->to_die = philo.to_die;
			thread[i]->to_eat = philo.to_eat;
			thread[i]->to_sleep = philo.to_sleep;
			thread[i]->slept = 0;
			thread[i]->think = 0;
			thread[i]->fork = 0;
			thread[i]->life = 0;
			thread[i]->time = philo.how_mutch;
			if (pthread_create(&thread[i]->threads, NULL, &func, thread[i]) != 0)
				printf("Thread creation failed \n");
		i += 1;
	}
	if (pthread_create(&pt, NULL, &checkout_, NULL) != 0)
		printf("Thread creation failed\n");
	if (pthread_join(pt, NULL) != 0)
		printf("Join failed\n");
}

int main(int ac, char **av)
{
    t_philo philo;

    memset(&philo, 0, sizeof(t_philo));
	philo.how_mutch = -1;
	checkout = 0;
    if (ac == 5)
    {
        philo.philo = atoi(av[1]);
        philo.to_die = atoi(av[2]);
		philo.to_eat = atoi(av[3]);
        philo.to_sleep = atoi(av[4]);
        philosopher(philo);
    }
    else if (ac == 6)
    {
		philo.philo = atoi(av[1]);
		philo.to_die = atoi(av[2]);
		philo.to_eat = atoi(av[3]);
		philo.to_sleep = atoi(av[4]);
		philo.how_mutch = atoi(av[5]);
        philosopher(philo);
    }
    else
        write(2, "Usage: 200 100 100 100 / 5\n", 27);
    return (0);
}
