/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:21:27 by barto             #+#    #+#             */
/*   Updated: 2025/02/28 14:16:14 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	find_env_var_index(char **env, char *name)
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

char	*get_env_var(t_environment *env, const char *name)
{
	t_environment	*current;

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

void	update_env_var(t_terminal *term, const char *name, const char *value)
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
