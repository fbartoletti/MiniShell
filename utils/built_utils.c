/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:55:16 by barto             #+#    #+#             */
/*   Updated: 2025/02/05 15:41:39 by fbartole         ###   ########.fr       */
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

void update_pwd_env(t_minishell *shell, const char *old_pwd)
{
	char	*pwd;
	char	*new_pwd;
	char	*old_pwd_var;
	char	*args[3];

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		new_pwd = ft_strjoin("PWD=", pwd);
		if (new_pwd)
		{
			args[0] = "export";
			args[1] = new_pwd;
			args[2] = NULL;
			ft_export(shell, args);
			free(new_pwd);
		}
		old_pwd_var = ft_strjoin("OLDPWD=", old_pwd);
		if (old_pwd_var)
		{
			args[0] = "export";
			args[1] = old_pwd_var;
			args[2] = NULL;
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
	char	*name;
	char	*value;
	char	*env_string;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		name = ft_substr(arg, 0, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = ft_strdup("");
	}
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

