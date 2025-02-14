/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:30:41 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 14:51:30 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// utilizzata per cercare la prima occorrenza di un determinato byte
//in una porzione di memoria. (sz: numero byte da esaminare)

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t sz)
{
	size_t	i;

	i = 0;
	while (i < sz)
	{
		if (((unsigned char *)s)[i] == (const unsigned char)c)
			return ((void *)(s + i));
		i++;
	}
	return (0);
}
