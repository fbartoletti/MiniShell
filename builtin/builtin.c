/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:42:25 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 13:36:54 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	cmd_echo(t_terminal *term, char **args)
{
	int	n_flag;
	int	i;

	(void)term;
	n_flag = 0;
	i = 1;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	ft_cd_handle_path(t_terminal *term, char **args, char **path, char **old_pwd)
{
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
		return (1);
	if (!args[1])
	{
		*path = get_env_var(term->env, "HOME");
		if (!*path)
		{
			free(*old_pwd);
			return (1);
		}
	}
	else if (args[1][0] == '\0')
	{
		free(*old_pwd);
		*path = NULL;
		return (0);
	}
	else
		*path = ft_strdup_safe(args[1]);
	return (0);
}

int	cmd_cd(t_terminal *term, char **args)
{
	char	*path;
	char	*old_pwd;
	char	*absolute_path;
	int		ret;

	path = NULL;
	old_pwd = NULL;
	ret = ft_cd_handle_path(term, args, &path, &old_pwd);
	if (ret != 0)
		return (ret);
	absolute_path = realpath(path, NULL);
	if (!absolute_path)
	{
		free(path);
		return (1);
	}
	if (access(absolute_path, F_OK) != 0)
	{
		free(absolute_path);
		free(path);
		return (1);
	}
	return (change_directory(absolute_path, path, old_pwd));
}

int	cmd_pwd(t_terminal *term, char **args)
{
	char	path[4096];

	(void)term;
	(void)args;
	if (!getcwd(path, sizeof(path)))
	{
		display_error("pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", path);
	return (0);
}

int	cmd_exit(t_terminal *term, char **args)
{
	int	status;

	(void)term;
	ft_putendl_fd("exit", 2);
	if (!args[1])
	{
		status = g_last_status;
		exit(status);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		g_last_status = 1;
		return (1);
	}
	status = ft_atoi(args[1]);
	exit(status & 0xFF);
}
