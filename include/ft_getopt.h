/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:09:35 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/27 16:19:37 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GETOPT_H
# define FT_GETOPT_H

// Global variable to store argument of current option
char	*g_optarg;
// Global variable to store the index of the next argument to be processed
int		g_optind;

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


#endif