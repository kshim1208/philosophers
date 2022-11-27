NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm
RMFLAGS = -f

SRC_FILES	= main.c initialize.c philosophers.c philosophers_end.c surveil_eat.c surveil_end.c time.c util.c

SRCS = $(SRC_FILES)

OBJS = $(SRCS:.c=.o)

DEPS = $(SRCS:.c=.d)


all : $(NAME)

$(NAME): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o	: %.c %.d
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

%.d : %.c
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@ -MDD

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: 
	make fclean 
	make all

.PHONY: all bonus clean fclean re

-include $(DEPS)
