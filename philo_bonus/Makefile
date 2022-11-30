NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm
RMFLAGS = -f

SRC_FILES	= main_bonus.c initialize_bonus.c philosophers_bonus.c philosophers_fork_bonus.c philosophers_end_bonus.c surveil_napkin_bonus.c surveil_end_bonus.c time_bonus.c util_bonus.c

SRCS = $(SRC_FILES)

OBJS = $(SRCS:.c=.o)


all : $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@ 

clean:
	$(RM) $(RMFLAGS) $(OBJS)

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re
