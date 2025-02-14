/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:35 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 15:05:27 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = ft_strlen(str);
	if (c == 0)
		return ((char *)str + i);
	while (i >= 0)
	{
		if (str[i] == (const char)c)
			return ((char *)str + i);
		i--;
	}
	return (NULL);
}
