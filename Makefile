NAME = philosophers

CC = cc
FLAGS = -Wall -Wextra -Werror
DEBUG = -g
RM = rm
RMFLAGS = -f

HEADERS = -I$(INCLUDE)

BONUS_HEADERS = -I$(BONUS_INCLUDES)

INCLUDE = ./include

BONUS_INCLUDES = ./bonus_include

SRCDIR = ./src

BONUS_SRCDIR = ./bonus_src


BONUS_SRC_FILES	= main_bonus.c
BONUS_SRC_FILES	:= $(addprefix $(BONUS_SRCDIR)/, $(BONUS_SRC_FILES))

BONUS_SRCS = $(BONUS_SRC_FILES)


SRC_FILES	= main.c initialize.c philosophers.c util.c
SRC_FILES	:= $(addprefix $(SRC_DIR)/, $(SRC_FILES))

SRCS = $(SRC_FILES)


BONUS_OBJS = $(BONUS_SRCS:.c=.o)

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

