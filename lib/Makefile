# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/09 10:06:54 by tliangso          #+#    #+#              #
#    Updated: 2024/03/02 10:56:37 by tliangso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= libft.a

HEAD	= include

DIR = srcs



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

GETOPT_SRC = 	ft_getopt_long.c \
				ft_getopt.c \

EXTRA_SRC	= 	ft_lstsort.c \
	ft_strcmp.c \
	ft_strncpy.c \


LIBFT_SRCS = ${addprefix ${DIR}/libft/, ${LIBFT_SRC}}
GNL_SRCS = ${addprefix ${DIR}/gnl/, ${GNL_SRC}}
PRINTF_SRCS = ${addprefix ${DIR}/printf/, ${PRINTF_SRC}}
EXTRA_SRCS = ${addprefix ${DIR}/libft/, ${EXTRA_SRC}}
HASHTABLE_SRCS = ${addprefix ${DIR}/ht/, ${HASHTABLE_SRC}}
GETOPT_SRCS = ${addprefix ${DIR}/getopt/, ${GETOPT_SRC}}


SRCS	= ${LIBFT_SRCS} \
		${GNL_SRCS} \
		${PRINTF_SRCS} \
		${EXTRA_SRCS} \
		${HASHTABLE_SRCS} \
		${GETOPT_SRCS} \
		

OBJS	= ${SRCS:.c=.o}

CC		= cc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -O3

.c.o:
			${CC} ${CFLAGS} -c -I ${HEAD} $< -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS}
			ar rc ${NAME} ${OBJS}
			ranlib ${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean	all

.PHONY:		all	clean	fclean	re
