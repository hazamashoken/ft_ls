/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <tliangso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 11:40:40 by tliangso          #+#    #+#             */
/*   Updated: 2024/02/17 14:09:17 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// DESCRIPTION
//        These  functions  convert  lowercase letters to uppercase, and
//        vice versa.
//
//        If c is a lowercase letter, toupper()  returns  its  uppercase
//        equivalent,  if an uppercase representation exists in the cur‐
//        rent locale.  Otherwise, it returns c.  The toupper_l()  func‐
//        tion  performs  the same task, but uses the locale referred to
//        by the locale handle locale.
//
//        If c is an uppercase letter, tolower() returns  its  lowercase
//        equivalent,  if  a lowercase representation exists in the cur‐
//        rent locale.  Otherwise, it returns c.  The tolower_l()  func‐
//        tion  performs  the same task, but uses the locale referred to
//        by the locale handle locale.
//
//        If c is neither an unsigned char value nor EOF,  the  behavior
//        of these functions is undefined.
//
//        The  behavior  of  toupper_l() and tolower_l() is undefined if
//        locale is the special locale object LC_GLOBAL_LOCALE (see  du‐
//        plocale(3)) or is not a valid locale object handle.
//
// RETURN VALUE
//        The  value  returned  is that of the converted letter, or c if
//        the conversion was not possible.
#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 26 + 6);
	return (c);
}
