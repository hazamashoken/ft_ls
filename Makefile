# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/03 23:06:04 by tliangso          #+#    #+#              #
#    Updated: 2024/03/04 12:18:30 by tliangso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

INCLUDE := -I include -I lib/include


DIR := ./srcs
BUILD_DIR := ./build

LIB_NAME := libft.a

LIB := -L lib -lft

CC := cc
RM	:= rm -rf


SRC := main.c \
	date.c

CFLAGS := -Wall -Wextra -Werror
CFLAGS += -MMD -MP

CFLAGS += $(INCLUDE)
# CFLAGS += -O3
CFLAGS += -fsanitize=address -g

# DO NOT CHANGE
SRCS := ${addprefix ${DIR}/, ${SRC}}
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(BUILD_DIR)/$(NAME)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(NAME): $(OBJS) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB)
	@echo -e "\033[32m$(NAME) compiled\033[0m"

$(LIB_NAME):
	${MAKE} -C lib all

clean:
	${MAKE} -C lib clean
	${RM} $(BUILD_DIR)

fclean: clean
	${MAKE} -C lib fclean
	${RM} $(NAME)


re: fclean all

.PHONY: all clean fclean re lib

-include $(DEPS)