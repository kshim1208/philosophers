NAME = philo

CC = cc
FLAGS = -Wall -Wextra -Werror
DEBUG = -g
RM = rm
RMFLAGS = -f

SRC_FILES	= main.c initialize.c philosophers.c philosophers_end.c surveil_eat.c surveil_end.c time.c util.c

SRCS = $(SRC_FILES)

OBJS = $(SRCS:.c=.o)


ifdef DO_BONUS
	OBJECTS = $(BONUS_OBJS)
else
	OBJECTS = $(OBJS)
endif


all : $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJECTS) -o $(NAME)

%.o	: %.c
	$(CC) $(CFLAGS) $(DEBUG) $(HEADERS) -c $< -o $@

bonus: 
	make DO_BONUS=1 all
	
clean:
	$(RM) $(RMFLAGS) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re

