/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:07 by fbartole          #+#    #+#             */
/*   Updated: 2024/02/06 15:29:51 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* char **ft_split(char const *s, char c);
Parameters
s: The string to be split.
c: The delimiter character.

Return value
The array of new strings resulting from the split.
NULL if the allocation fails.

Description Allocates (with malloc(3)) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter. The array must end
with a NULL pointer. */

#include "libft.h"

static size_t	ft_strcounter(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

static char	*allocate_and_copy(const char *str, char c)
{
	size_t	len;
	char	*result;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	result = (char *)malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, len + 1);
	return (result);
}

char	**ft_split(const char *str, char c)
{
	size_t	words_count;
	char	**matrix;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	words_count = ft_strcounter(str, c);
	matrix = (char **)malloc((words_count + 1) * sizeof(char *));
	if (!matrix)
		return (NULL);
	i = 0;
	j = 0;
	while (j < words_count)
	{
		while (str[i] == c)
			i++;
		matrix[j++] = allocate_and_copy(&str[i], c);
		if (!matrix[j - 1])
			return (NULL);
		while (str[i] && str[i] != c)
			i++;
	}
	matrix[j] = NULL;
	return (matrix);
}
