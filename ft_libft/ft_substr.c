/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:40 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/05 15:09:47 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *str, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	if (start >= ft_strlen(str))
		return (ft_strdup(""));
	if (len > ft_strlen(str) || (len + start) > ft_strlen(str))
		len = ft_strlen(str) - start;
	sub_str = (char *)malloc((len + 1) * sizeof(char));
	if (!sub_str)
		return (NULL);
	i = start;
	j = 0;
	while (i < ft_strlen(str) && j < len)
	{
		sub_str[j] = str[i];
		i++;
		j++;
	}
	sub_str[j] = '\0';
	return (sub_str);
}
