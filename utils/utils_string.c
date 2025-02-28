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
	dup = alloc_mem(len + 1);
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

void	add_new_env_var(t_terminal *term, char *env_string)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (term->new_env && term->new_env[i])
		i++;
	new_env = alloc_mem((i + 2) * sizeof(char *));
	j = 0;
	while (j < i)
	{
		new_env[j] = term->new_env[j];
		j++;
	}
	new_env[i] = env_string;
	new_env[i + 1] = NULL;
	if (term->new_env)
		free(term->new_env);
	term->new_env = new_env;
}

int find_env_var_index(char **env, char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (-1);

	i = 0;
	len = ft_strlen(name);
	
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char *get_env_var(t_environment *env, const char *name)
{
	t_environment *current;
	
	if (!env || !name)
		return (NULL);
		
	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void update_env_var(t_terminal *term, const char *name, const char *value)
{
	int		index;
	char	*new_var;
	char	*temp;
	
	if (!term || !name || !value || !term->new_env)
		return;
		
	index = find_env_var_index(term->new_env, (char *)name);
	
	if (index >= 0)
	{
		temp = ft_strjoin(name, "=");
		new_var = ft_strjoin(temp, value);
		free(temp);
		
		free(term->new_env[index]);
		term->new_env[index] = new_var;
	}
	else
	{
		temp = ft_strjoin(name, "=");
		new_var = ft_strjoin(temp, value);
		free(temp);
		
		add_new_env_var(term, new_var);
	}
}
