NAME = philo

CC = cc
FLAGS = -Wall -Wextra -Werror
DEBUG = -g
RM = rm
RMFLAGS = -f

SRC_FILES	= main.c initialize.c philosophers.c philosophers_end.c surveil_eat.c surveil_end.c time.c util.c

SRCS = $(SRC_FILES)

OBJS = $(SRCS:.c=.o)


all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJS) -o $(NAME)

%.o	: %.c
	$(CC) $(CFLAGS) $(DEBUG) $(HEADERS) -c $< -o $@

	
clean:
	$(RM) $(RMFLAGS) $(OBJS)

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re

