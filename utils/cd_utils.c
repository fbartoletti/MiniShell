/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:24:43 by barto             #+#    #+#             */
/*   Updated: 2025/03/06 09:52:10 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	change_directory(char *absolute_path, char *path, char *old_pwd)
{
	int		ret;
	char	*new_pwd;

	ret = chdir(absolute_path);
	if (ret < 0)
	{
		free(absolute_path);
		free(path);
		return (1);
	}
	if (old_pwd)
	{
		setenv("OLDPWD", old_pwd, 1);
		free(old_pwd);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		setenv("PWD", new_pwd, 1);
		free(new_pwd);
	}
	free(absolute_path);
	free(path);
	return (0);
}

int	cmd_export(t_terminal *term, char **args)
{
	int	i;

	if (!args[1])
		return (print_export_env(term));
	i = 1;
	while (args[i])
	{
		if (export_handle_arg(term, args[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	cmd_unset(t_terminal *term, char **args)
{
	int	i;
	int	env_index;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		env_index = find_env_var_index(term->new_env, args[i]);
		if (env_index >= 0)
		{
			free(term->new_env[env_index]);
			while (term->new_env[env_index])
			{
				term->new_env[env_index] = term->new_env[env_index + 1];
				env_index++;
			}
		}
		i++;
	}
	update_env_list_after_unset(term, args);
	return (0);
}

int	cmd_env(t_terminal *term, char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (term->new_env[i])
	{
		if (ft_strchr(term->new_env[i], '='))
			ft_putendl_fd(term->new_env[i], 1);
		i++;
	}
	return (0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
