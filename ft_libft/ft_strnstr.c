/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:33 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/06 16:59:42 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//utilizzata per cercare la prima occorrenza di una sottostringa
//all'interno di una stringa principale, considerando al massimo
//i primi n caratteri della stringa principale
// con len = Il massimo numero di caratteri di to_find da considerare.
#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!to_find[0] || to_find == NULL)
		return ((char *)str);
	if (len == 0)
		return (NULL);
	while (i < len && str[i])
	{
		j = 0;
		while ((i + j) < len && to_find[j] && str[i + j] == to_find[j])
		{
			if (to_find[j + 1] == '\0')
				return ((char *)(str + i));
			j++;
		}
		i++;
	}
	return (NULL);
}
