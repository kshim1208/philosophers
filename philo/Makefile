.DEFAULT_GOAL := all

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

DEPENDENCY = -MMD -MP

RM = rm
RMFLAGS = -f

SRC_FILES	= main.c initialize.c philosophers.c philosophers_end.c surveil_napkin.c surveil_end.c time.c util.c

SRCS = $(SRC_FILES)

OBJS = $(SRCS:.c=.o)

DEPS = $(SRCS:.c=.d)

-include $(DEPS) 


all : $(NAME)

$(NAME): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) $(DEPENDENCY) $(HEADERS) -c $< -o $@ 

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re
