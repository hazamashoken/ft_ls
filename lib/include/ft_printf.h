/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 02:43:35 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/03 22:51:37 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdlib.h>
# include <limits.h>
# include <stdarg.h>
# include <unistd.h>
# include <stdint.h>

# define FORMAT "cspdiuxX%"
# define NULL_STR "(null)"

typedef struct s_format
{
	va_list	args;
	long	width;
	int		precision;
	int		zero;
	int		dot;
	int		minus;
	int		total_len;
	int		plus;
	int		is_zero;
	int		percent;
	int		space;
	int		hash;
	char	type;
	int		fd;
}	t_format;

//printf.c
int		ft_printf(const char *format, ...);
int		ft_dprintf(int fd, const char *format, ...);

//ft_printf_utils.c
int		ft_istype(char c);

//ft_checker.c
void	print_format(t_format *fmt);

//ft_star.c
int		star_flag(char c, t_format *fmt);
//ft_hex.c
size_t	ft_uintlen(unsigned int nb, int div);
char	*get_hexaddr_str(size_t x, char format);
char	*get_hex_str(unsigned int x, char format);

//flag_str.c
char	*plus_flag_str(char *str, t_format *fmt);
char	*hash_flag_str(char *str, t_format *fmt);
char	*format_pcs_digit(char *str, t_format *fmt);
char	*format_pcs_str(char *str, t_format *fmt);

//ft_type_str.c
char	*c_type_str(int c);
char	*s_type_str(char *s);
char	*p_type_str(size_t p);
char	*d_type_str(int d);
char	*u_type_str(unsigned int u);

//ft_type_str2.c
char	*x_type_str(unsigned int x, char type);

//utils.c
char	*ft_strjoin_free(char *s1, char *s2);

//u_type_utils.c
char	*uint_str(unsigned int d);

//precision_digit.c
char	*pcs_digit_operation(char *str, t_format *fmt);

//printer.c
int		print_str(char *cstr, t_format *fmt);
int		printf_empty(char *str, t_format *fmt);
int		print_char(int fd, char *str, int w);

//align.c
char	*align_left(char *str, t_format *fmt);
char	*align_right(char *str, t_format *fmt);
int		printlen(char *str);

#endif
