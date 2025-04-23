/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:07:17 by lagea             #+#    #+#             */
/*   Updated: 2024/08/16 12:52:50 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

static void	skip_whitespace_and_sign(const char **str, int *sign)
{
	while ((**str >= 9 && **str <= 13) || **str == 32)
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	res;
	long	overflow;

	res = 0;
	sign = 1;
	overflow = 0;
	skip_whitespace_and_sign(&str, &sign);
	while (ft_isdigit(*str))
	{
		res *= 10;
		res += (*str - 48);
		str++;
		if (overflow > res && sign == 1)
			return (-1);
		else if (overflow > res && sign == -1)
			return (0);
		overflow = res;
	}
	return ((int)res * sign);
}
