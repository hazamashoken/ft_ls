DIR := srcs/

BUILD_DIR := build/

SRC := main.c \
	date.c




SRCS := ${addprefix ${DIR}, ${SRC}}

OBJS := ${SRCS:.c=.o}

INCLUDE := -I include -I lib/include

LIB := -L lib -lft

CC := cc
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -MMD

CFLAGS += $(INCLUDE)
# CFLAGS += -O3
CFLAGS += -fsanitize=address -g

NAME = ft_ls

DEPS := $(OBJS:.o=.d)

-include $(DEPS)


all: lib $(NAME)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)

lib:
	make -C lib all


clean:
	make -C lib clean
	rm -f $(OBJS) $(DEPS)

fclean: clean
	make -C lib fclean
	rm -f $(NAME)



re: fclean all

.PHONY: all clean fclean re lib