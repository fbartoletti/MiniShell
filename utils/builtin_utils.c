/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:59:15 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 10:42:12 by barto            ###   ########.fr       */
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

int	find_env_index(char **env, char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	ft_export(t_minishell *shell, char **args)
{
	int	i;

	if (!args[1])
		return ft_export_print_env(shell);
	i = 1;
	while (args[i])
	{
		if (ft_export_handle_arg(shell, args[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_unset(t_minishell *shell, char **args)
{
	int	i;
	int	env_index;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		env_index = find_env_index(shell->env, args[i]);
		if (env_index >= 0)
		{
			free(shell->env[env_index]);
			while (shell->env[env_index])
			{
				shell->env[env_index] = shell->env[env_index + 1];
				env_index++;
			}
		}
		i++;
	}
	return (0);
}

int	ft_env(t_minishell *shell, char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}
