/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:38 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/06 15:36:25 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_allocation(int len)
{
	char	*trim_str;

	trim_str = (char *)malloc((len + 1) * sizeof(char));
	if (!trim_str)
		return (NULL);
	return (trim_str);
}

static int	ft_trimmer(char c, const char *s2)
{
	while (*s2)
	{
		if (c == *s2++)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *s2)
{
	char	*trim_str;
	size_t	i;
	size_t	j;
	size_t	z;

	if (!s1)
		return (NULL);
	z = 0;
	i = 0;
	j = ft_strlen(s1);
	if (j == 0)
		return (ft_strdup(""));
	while (ft_trimmer(s1[i], s2))
		i++;
	while (j > 0 && ft_trimmer(s1[j - 1], s2))
		j--;
	if (i >= j)
		return (ft_strdup(""));
	trim_str = ft_allocation(j - i);
	if (!trim_str)
		return (NULL);
	while (i < j)
		trim_str[z++] = s1[i++];
	trim_str[z] = '\0';
	return (trim_str);
}
