/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:21:27 by barto             #+#    #+#             */
/*   Updated: 2025/02/05 15:29:29 by fbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(s);
	dup = safe_malloc(len + 1);
	if (!dup)
		return (NULL);
	return (ft_strcpy(dup, s));
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

void add_new_env_var(t_minishell *shell, char *env_string)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (shell->env[i])
		i++;
	new_env = safe_malloc((i + 2) * sizeof(char *));
	j = 0;
	while (j < i)
	{
		new_env[j] = shell->env[j];
		j++;
	}
	new_env[i] = env_string;
	new_env[i + 1] = NULL;
	shell->env = new_env;
}
