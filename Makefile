# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: molabhai <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/30 13:19:05 by molabhai          #+#    #+#              #
#    Updated: 2021/06/30 13:19:07 by molabhai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = philo

CC = gcc

FLAGS = -Wall -Wextra -Werror

SRCS = main.c

OBJ = $(SRCS:.c=.o)

all: $(NAME)
$(NAME):
	$(CC) $(FLAGS) -lpthread -pthread $(SRCS) -o philo

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all