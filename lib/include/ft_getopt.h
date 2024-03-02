/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:09:35 by tliangso          #+#    #+#             */
/*   Updated: 2024/03/02 10:59:07 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H

#include "libft.h"

// Global variable to store argument of current option
extern char	*g_optarg;
// Global variable to store the index of the next argument to be processed
extern int	g_optind;

# define NO_ARG 0
# define REQUIRED_ARG 1
# define OPTIONAL_ARG 2

# define no_argument 0
# define required_argument 1
# define optional_argument 2


/**
 *
```
typedef struct s_option {
	const char	*name;
	int			has_arg;
	int			*flag;
	int			val;
}			t_option;
```
*/
typedef struct s_option {
	const char	*name;
	int			has_arg;
	int			*flag;
	int			val;
}			t_option;

int	ft_getopt(int argc, char *const argv[], const char *optstring);
int	ft_getopt_long(int argc, char *const argv[], const char *optstring, \
	const t_option *longopts, int *longindex);

#endif