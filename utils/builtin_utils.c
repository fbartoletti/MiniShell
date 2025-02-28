/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:59:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/07 15:52:06 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*create_env_string(char *name, char *value)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
}

int	print_export_env(t_terminal *term)
{
	char	**sorted_env;
	int		env_size;
	int		i;

	env_size = 0;
	while (term->new_env[env_size])
		env_size++;
	sorted_env = alloc_mem(sizeof(char *) * (env_size + 1));
	i = 0;
	while (i < env_size)
	{
		sorted_env[i] = ft_strdup_safe(term->new_env[i]);
		i++;
	}
	sorted_env[env_size] = NULL;
	sort_env_array(sorted_env, env_size);
	i = 0;
	while (sorted_env[i])
	{
		print_export_var(sorted_env[i]);
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
	return (0);
}

int	handle_export_error(char *name, char *value)
{
	display_error("export: not a valid identifier");
	free(name);
	free(value);
	return (1);
}

void	update_env_list_after_unset(t_terminal *term, char **args)
{
	t_environment	*current;
	t_environment	*next;
	int				i;
	
	i = 1;
	while (args[i])
	{
		current = term->env;
		while (current)
		{
			next = current->next;
			if (ft_strcmp(current->name, args[i]) == 0)
			{
				if (current->prev != current)
				{
					if (current == term->env)
						term->env = current->next;
					if (current->prev)
						current->prev->next = current->next;
					if (current->next)
						current->next->prev = current->prev;
				}
				else
					term->env = NULL;
				free(current->var);
				free(current->name);
				if (current->value)
					free(current->value);
				free(current);
				break;
			}
			current = next;
		}
		i++;
	}
}
