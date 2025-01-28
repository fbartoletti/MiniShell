/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:55:16 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 12:34:02 by barto            ###   ########.fr       */
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

void	update_pwd_env(t_minishell *shell, const char *old_pwd)
{
	char	*pwd;
	char	*new_pwd;
	char	*old_pwd_var;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		new_pwd = ft_strjoin("PWD=", pwd);
		if (new_pwd)
		{
			char *args[] = {"export", new_pwd, NULL};
			ft_export(shell, args);
			free(new_pwd);
		}
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (old_pwd_var)
		{
			char *args[] = {"export", old_pwd_var, NULL};
			ft_export(shell, args);
			free(old_pwd_var);
		}
		free(pwd);
	}
}

int ft_export_print_env(t_minishell *shell)
{
	int i;

	i = 0;
	while (shell->env[i])
		printf("declare -x %s\n", shell->env[i++]);
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
	char	*equals;
	char	*name;
	char	*value;
	char	*env_string;

	equals = ft_strchr(arg, '=');
	if (!equals)
		return (0);
	name = ft_substr(arg, 0, equals - arg);
	value = ft_strdup(equals + 1);
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
