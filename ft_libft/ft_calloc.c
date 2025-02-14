/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:30:25 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/06 17:06:09 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	tot_size;
	void	*dest;

	tot_size = count * size;
	if (count && size && (4294967295U / size <= count))
		return (NULL);
	dest = malloc(tot_size);
	if (!dest)
		return (NULL);
	ft_bzero(dest, (tot_size));
	return (dest);
}
