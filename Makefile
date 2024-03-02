DIR = srcs/

BUILD = build/

SRC = main.c \
	date.c




SRCS = ${addprefix ${DIR}, ${SRC}}

OBJS = ${SRCS:.c=.o}

INCLUDE = -I include -I lib/include

LIB = -L lib -lft

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
	make -C lib all


clean:
	make -C lib clean
	rm -f $(OBJS)

fclean: clean
	make -C lib fclean
	rm -f $(NAME)



re: fclean all

.PHONY: all clean fclean re lib