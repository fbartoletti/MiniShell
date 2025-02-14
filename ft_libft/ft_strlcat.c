/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:20 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 15:02:58 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// concatena 2 stringhe e ti torna la len della string concatenata
size_t	ft_strlcat(char *dest, const char *src, size_t destsize)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (destsize == 0)
		return (ft_strlen(src));
	while (dest[i] != '\0' && i < destsize)
		i++;
	while ((i + j + 1) < destsize && src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i != destsize)
		dest[i + j] = '\0';
	return (i + ft_strlen(src));
}
