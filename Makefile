DIR = srcs/

BUILD = build/

SRC = main.c

SRCS = ${addprefix ${DIR}, ${SRC}}

OBJS = ${SRCS:.c=.o}

INCLUDE = -I include

LIB = -L lib/libft/ -lft

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDE)

NAME = ft_ls



all: lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)

lib:
	make -C lib/libft all


clean:
	make -C lib/libft/ clean

	rm -f $(NAME)

fclean: clean
	make -C lib/libft/ fclean



re: fclean all

.PHONY: all clean fclean re lib