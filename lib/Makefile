# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/09 10:06:54 by tliangso          #+#    #+#              #
#    Updated: 2024/03/05 00:34:21 by tliangso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= libft.a

INC_DIR	= include

CC		= cc
RM		= rm -rf

DIR = ./srcs
BUILD_DIR := ./build

LIBFT_SRC		=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
			ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
			ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
			ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
			ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			ft_lstmap.c

GNL_SRC	=	get_next_line.c get_next_line_utils.c  \

PRINTF_SRC = ft_align.c \
			 ft_checker.c \
			 ft_flag_str.c \
			 ft_hex.c \
			 ft_precision_digit.c \
			 ft_printer.c \
			 ft_printf.c \
			 ft_printf_utils.c \
			 ft_star.c \
			 ft_type_str.c \
			 ft_type_str2.c \
			 ft_u_type_utils.c \
			 ft_utils.c

HASHTABLE_SRC = ht_set_get.c \
				ht_create.c \
				ht_iterator.c \
				ht_length.c \
				ht_next.c \
				ht_destory.c \

GETOPT_SRC = 	ft_getopt.c \

ARGMATCH_SRC = 	ft_argmatch.c \

EXTRA_SRC	= 	ft_lstsort.c \
	ft_strcmp.c \
	ft_strncpy.c \
	ft_basename.c \
	ft_quote.c \


LIBFT_SRCS = ${addprefix ${DIR}/libft/, ${LIBFT_SRC}}
GNL_SRCS = ${addprefix ${DIR}/gnl/, ${GNL_SRC}}
PRINTF_SRCS = ${addprefix ${DIR}/printf/, ${PRINTF_SRC}}
EXTRA_SRCS = ${addprefix ${DIR}/extra/, ${EXTRA_SRC}}
HASHTABLE_SRCS = ${addprefix ${DIR}/ht/, ${HASHTABLE_SRC}}
GETOPT_SRCS = ${addprefix ${DIR}/getopt/, ${GETOPT_SRC}}
ARGMATCH_SRCS = ${addprefix ${DIR}/argmatch/, ${ARGMATCH_SRC}}


SRCS	= ${LIBFT_SRCS} \
		${GNL_SRCS} \
		${PRINTF_SRCS} \
		${EXTRA_SRCS} \
		${HASHTABLE_SRCS} \
		${GETOPT_SRCS} \
		${ARGMATCH_SRCS}


OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


CFLAGS	= -Wall -Wextra -Werror
CFLAGS += -MMD -MP
# CFLAGS  += -fsanitize=address -g
CFLAGS  += -O3
CFLAGS  += -I $(INC_DIR)

DEPS := $(OBJS:.o=.d)

all:		${NAME}

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

${NAME}:	${OBJS}
			${AR} rc ${NAME} ${OBJS}
			ranlib ${NAME}
			@echo -e "\033[32m$(NAME) compiled\033[0m"

clean:
			${RM} ${BUILD_DIR}

fclean:		clean
			${RM} ${NAME}

re:			fclean	all

.PHONY:		all	clean	fclean	re

-include $(DEPS)