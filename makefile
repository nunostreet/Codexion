NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c89 -pedantic-errors
CPPFLAGS = -I.
SRCS = main.c init.c time.c safe_functions.c getters_setters.c \
	coders.c sim.c monitor.c synchro_utils.c heap.c dongles.c \
	parsing/parsing.c parsing/parsing_numbers.c parsing/parsing_scheduler.c \
	utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) -pthread -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -pthread -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
