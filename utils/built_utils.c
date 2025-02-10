/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:55:16 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:13:43 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_var(char **env, const char *name)
{
	int		name_len;
	int		i;

	if (!name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (ft_strdup(env[i] + name_len + 1));
		i++;
	}
	return (NULL);
}

int	ft_export_print_env(t_minishell *shell)
{
	char	**sorted_env;
	int		env_size;
	int		i;

	env_size = 0;
	while (shell->env[env_size])
		env_size++;
	sorted_env = (char **)safe_malloc(sizeof(char *) * (env_size + 1));
	i = 0;
	while (i < env_size)
	{
		sorted_env[i] = ft_strdup(shell->env[i]);
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

int	ft_export_create_or_update_env(t_minishell *shell, char *name, char *env_string)
{
	int	env_index;

	env_index = find_env_index(shell->env, name);
	if (env_index >= 0)
	{
		free(shell->env[env_index]);
		shell->env[env_index] = env_string;
	}
	else
	{
		add_new_env_var(shell, env_string);
	}
	return 0;
}

int	ft_export_handle_arg(t_minishell *shell, char *arg)
{
	char	*name;
	char	*value;
	char	*env_string;

	if (extract_name_value(arg, &name, &value))
		return (1);
	if (!is_valid_identifier(name))
		return (handle_error(name, value));
	if (!ft_strchr(arg, '='))
		return (handle_no_equal(shell, name, value));
	env_string = create_env_string(name, value);
	if (!env_string)
	{
		free(name);
		free(value);
		return (1);
	}
	ft_export_create_or_update_env(shell, name, env_string);
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
