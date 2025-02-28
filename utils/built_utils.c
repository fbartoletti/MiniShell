/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:55:16 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 10:15:43 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_export_print_env(t_terminal *term)
{
	char	**sorted_env;
	int		env_size;
	int		i;

	env_size = 0;
	while (term->new_env[env_size])
		env_size++;
	sorted_env = (char **)alloc_mem(sizeof(char *) * (env_size + 1));
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

int	ft_export_create_or_update_env(t_terminal *term, char *name, char *env_string)
{
	int	env_index;

	env_index = find_env_var_index(term->new_env, name);
	if (env_index >= 0)
	{
		free(term->new_env[env_index]);
		term->new_env[env_index] = env_string;
	}
	else
	{
		add_new_env_var(term, env_string);
	}
	return (0);
}

int	export_handle_arg(t_terminal *term, char *arg)
{
	char	*name;
	char	*value;
	char	*env_string;

	if (extract_name_value(arg, &name, &value))
		return (1);
	if (!is_valid_identifier(name))
		return (handle_export_error(name, value));
	if (!ft_strchr(arg, '='))
		return (handle_no_equal(term, name, value));
	env_string = create_env_string(name, value);
	if (!env_string)
	{
		free(name);
		free(value);
		return (1);
	}
	update_env_var(term, name, value);
	free(name);
	free(value);
	return (0);
}

int	extract_name_value(char *arg, char **name, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*name = ft_substr(arg, 0, equal_pos - arg);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*name = ft_strdup(arg);
		*value = ft_strdup("");
	}
	return (*name && *value) ? 0 : 1;
}
