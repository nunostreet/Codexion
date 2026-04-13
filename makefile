NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I.
SRCS = main.c init.c parsing/parsing.c parsing/parsing_numbers.c \
	parsing/parsing_scheduler.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
