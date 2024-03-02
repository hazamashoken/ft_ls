DIR = srcs/

BUILD = build/

SRC = main.c \
	date.c \
	ht/ht_set_get.c \
	ht/ht_create.c \
	ht/ht_iterator.c \
	ht/ht_length.c \
	ht/ht_next.c \
	ht/ht_destory.c \
	getopt/ft_getopt_long.c \
	getopt/ft_getopt.c \



SRCS = ${addprefix ${DIR}, ${SRC}}

OBJS = ${SRCS:.c=.o}

INCLUDE = -I include -I lib/include

LIB = -L lib/libft/ -lft

CC = cc
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -O3
# CFLAGS += -fsanitize=address -g
CFLAGS += $(INCLUDE)
NAME = ft_ls



all: lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)

lib:
	make -C lib/libft all


clean:
	make -C lib/libft/ clean
	rm -f $(OBJS)

fclean: clean
	make -C lib/libft/ fclean
	rm -f $(NAME)



re: fclean all

.PHONY: all clean fclean re lib