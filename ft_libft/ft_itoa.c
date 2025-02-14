/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:30:38 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 15:32:03 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Returns a string
representing the integer received as an argument.
Negative numbers must be handled.
*/

#include "libft.h"

static char	*pre_conv(int len);
static int	ft_int_len(long nbr);

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	nbr;

	nbr = n;
	len = ft_int_len(nbr);
	result = pre_conv(len);
	if (!result)
		return (NULL);
	if (nbr < 0)
		nbr = -nbr;
	i = len - 1;
	while (nbr != 0)
	{
		result[i] = ((nbr % 10) + 48);
		nbr = nbr / 10;
		i--;
	}
	if (n < 0)
		result[0] = '-';
	result[len] = '\0';
	return (result);
}

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	tmp[0] = '0';
	return (tmp);
}

static int	ft_int_len(long nbr)
{
	int	digit_count;

	digit_count = 0;
	if (nbr < 0)
	{
		digit_count++;
		nbr = -nbr;
	}
	if (nbr == 0)
		digit_count++;
	while (nbr != 0)
	{
		nbr /= 10;
		digit_count++;
	}
	return (digit_count);
}
