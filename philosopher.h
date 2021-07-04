/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molabhai <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 18:38:04 by molabhai          #+#    #+#             */
/*   Updated: 2021/06/29 18:38:07 by molabhai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

typedef struct s_thread
{
    int             fork;
    int             philo_nmbr;
    int             to_die;
    int             to_eat;
    int             to_sleep;
	pthread_t		threads;
	int 			id;
    int             eat;
    int             slept;
    int             think;
    long            life;
    pthread_mutex_t *mutex;
}				t_thread;

typedef struct s_mutexes
{
    pthread_mutex_t mutex;
    int             id;
}               t_mutexes;

typedef struct s_philo
{
    int				philo;
    int             *index;
    t_thread 		*thread;
    t_mutexes       **mutex;
    int				to_die;
    int				to_eat;
    int				to_sleep;
	int				how_mutch;
    int				starting_time;
   // pthread_mutex_t *mutex;
}               t_philo;