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

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

void    ft_putstr(char *s)
{
    while (*(s++) != '\0')
        ft_putchar(*(s));
}

void    *func(void *ptr)
{
	t_thread *thread;
	struct  timeval current_time;
	long current;
	
	int		avail_fork;
	int		check;


	thread = (t_thread *) ptr;
	avail_fork = thread->philo_nmbr;
	check = 0;
	gettimeofday(&current_time, NULL);
	thread->life = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	if ((thread->id % 2) == 0)
		usleep(thread->to_eat * 1000);
	gettimeofday(&current_time, NULL);
	while (check == 0)
	{
		//usleep(2000);
		current = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
		if ((current - thread->life) >= thread->to_die)
		{
			printf("%ld %d died\n", current, thread->id);
			check = 1;
			exit (3);
		}
		if (thread->eat == 0)
		{
			if ((thread->id >= 0 && thread->id < thread->philo_nmbr - 1) && avail_fork > 0)
			{
				if (pthread_mutex_lock(&thread->mutex[thread->id]) != 0)
					printf("Error in mutex id ==> %d\n", thread->id);
				else
				{
					thread->fork += 1;
					avail_fork -= 1;
					printf("%ld %d has take a fork\n", current, thread->id);
				}
				if (pthread_mutex_lock(&thread->mutex[thread->id + 1]) != 0)
					printf("Error in mutex id ==> %d\n", thread->id);
				else
				{
					thread->fork += 1;
					avail_fork -= 1;
					printf("%ld %d has take a fork\n", current, thread->id);
				}
			}
			else if ((thread->id == thread->philo_nmbr - 1) && avail_fork > 0)
			{
				if (pthread_mutex_lock(&thread->mutex[0]) != 0)
					printf("Error in mutex id ==> %d\n", thread->id);
				else
				{
					thread->fork += 1;
					avail_fork -= 1;
					printf("%ld %d has take a fork\n", current, thread->id);
				}
				if (pthread_mutex_lock(&thread->mutex[thread->philo_nmbr - 1]) != 0)
					printf("Error in mutex id ==> %d\n", thread->id);
				else
				{
					thread->fork += 1;
					avail_fork -= 1;
					printf("%ld %d has take a fork\n", current, thread->id);
				}
			}
			if (thread->fork == 2)
			{
				printf("%ld %d is eating\n", current, thread->id);
				thread->life = current;
				usleep(thread->to_eat * 1000);
				gettimeofday(&current_time, NULL);
				current = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
				thread->eat = 1;
				thread->fork = 0;
			}
			else
			{
				printf("ERRROR DeadLock\n");
				break ; 
			}
			if (thread->id == 0 || thread->id == thread->philo_nmbr - 1)
			{
				if (pthread_mutex_unlock(&thread->mutex[0]) != 0)
					printf("Error in unlock mutex id ==> %d\n", thread->id);
				avail_fork += 1;
				if (pthread_mutex_unlock(&thread->mutex[thread->philo_nmbr - 1]) != 0)
					printf("Error in unlock mutex id ==> %d\n", thread->id);
				avail_fork += 1;
			}
			if (thread->id > 0 && thread->id < thread->philo_nmbr - 1)
			{
				if (pthread_mutex_unlock(&thread->mutex[thread->id]) != 0)
					printf("Error in unlockmutex id ==> %d\n", thread->id);
				avail_fork += 1;
				if (pthread_mutex_unlock(&thread->mutex[thread->id + 1]) != 0)
					printf("Error in unlock mutex id ==> %d\n", thread->id);
				avail_fork += 1;
			}
		}
		if (thread->slept == 0 && thread->eat == 1)
		{
			printf("%ld %d is sleeping\n", current, thread->id);
			usleep(thread->to_sleep * 1000);
				gettimeofday(&current_time, NULL);
				current = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
			thread->slept = 1;
		}
		if (thread->slept == 1 && thread->eat == 1)
		{
			printf("%ld %d is thinking\n", current, thread->id);
			usleep(thread->think * 1000);
			gettimeofday(&current_time, NULL);
			current = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
			thread->slept = 0;
			thread->eat = 0;
		}
		gettimeofday(&current_time, NULL);
	}
	return(ptr);
}

void    philosopher(t_philo philo)
{
	int i;
	int	j;
	t_thread **thread;
	pthread_mutex_t *mutex;

	i = 0;
	j = 1;
	i = 0;
	mutex = malloc(sizeof(pthread_mutex_t) * philo.philo);
	thread = malloc(sizeof(t_thread *) * philo.philo);
	while (i < philo.philo)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	i = 0;
	philo.mutex = malloc(sizeof(t_mutexes *) * philo.philo);
	int check = 0;
	while (i < philo.philo)
	{
			thread[i] = malloc(sizeof(t_thread));
			thread[i]->id = i;
			thread[i]->mutex = mutex;
			thread[i]->philo_nmbr = philo.philo;
			thread[i]->to_die = philo.to_die;
			thread[i]->to_eat = philo.to_eat;
			thread[i]->to_sleep = philo.to_sleep;
			thread[i]->slept = 0;
			thread[i]->think = 0;
			thread[i]->fork = 0;
			thread[i]->life = 0;
			if (pthread_create(&thread[i]->threads, NULL, &func, thread[i]) != 0)
				printf("Thread creation failed \n");
		i += 1;
	}
	check = 0;
	i = 0;
	while (i < philo.philo)
	{
			if (pthread_join(thread[i]->threads, NULL) != 0)
				printf("Error in join\n");
		i += 1;
	}/*
	i = 0;
	while (i < philo.philo)
	{
		if ((i % 2) == 0)
		{
			thread[i] = malloc(sizeof(t_thread));
			thread[i]->id = i;
			thread[i]->mutex = mutex;
			thread[i]->philo_nmbr = philo.philo;
			thread[i]->to_die = philo.to_die;
			thread[i]->to_eat = philo.to_eat;
			thread[i]->to_sleep = philo.to_sleep;
			thread[i]->slept = 0;
			thread[i]->think = 0;
			thread[i]->fork = 0;
			thread[i]->life = 0;
			if (pthread_create(&thread[i]->threads, NULL, &func, thread[i]) != 0)
				printf("Thread creation failed \n");
		}
		i += 1;
	}
	i = 0;
	while (i < philo.philo)
	{
		if ((i % 2) == 0)
		{
			if (pthread_join(thread[i]->threads, NULL) != 0)
				printf("Error in join\n");
		}
		i += 1;
	}*/
}

int main(int ac, char **av)
{
    t_philo philo;

    memset(&philo, 0, sizeof(t_philo));
    if (ac == 5)
    {
        philo.philo = atoi(av[1]);
        philo.to_die = atoi(av[2]);
		philo.to_eat = atoi(av[3]);
        philo.to_sleep = atoi(av[3]);
        philosopher(philo);
    }
    else if (ac == 6)
    {
		philo.philo = atoi(av[1]);
		philo.to_die = atoi(av[2]);
		philo.to_eat = atoi(av[3]);
		philo.to_sleep = atoi(av[3]);
		philo.how_mutch = atoi(av[4]);
    }
    else
        write(2, "Usage: 200 100 100 100 / 5\n", 27);
    return (0);
}
