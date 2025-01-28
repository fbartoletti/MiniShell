/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:42:25 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 13:27:55 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_echo(t_minishell *shell, char **args)
{
	int	n_flag;
	int	i;

	(void)shell;
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

int	ft_cd_handle_path(t_minishell *shell, char **args, char **path, char **old_pwd)
{
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
		return 1;
	if (!args[1])
	{
		*path = get_env_var(shell->env, "HOME");
		if (!*path)
		{
			free(*old_pwd);
			return 1;
		}
	}
	else if (args[1][0] == '\0')
	{
		free(*old_pwd);
		*path = NULL;
		return 0;
	}
	else
		*path = ft_strdup(args[1]);
	return 0;
}

int	ft_cd(t_minishell *shell, char **args)
{
	char	*path;
	char	*old_pwd;
	char	*absolute_path;
	int		ret;

	path = NULL;
	old_pwd = NULL;
	ret = ft_cd_handle_path(shell, args, &path, &old_pwd);
	if (ret != 0)
		return ret;
	absolute_path = realpath(path, NULL);
	if (!absolute_path)
	{
		free(path);
		return 1;
	}
	if (access(absolute_path, F_OK) != 0)
	{
		free(absolute_path);
		free(path);
		return 1;
	}
	return change_directory(absolute_path, path, old_pwd);
}

int	ft_pwd(t_minishell *shell, char **args)
{
	char	path[4096];

	(void)shell;
	(void)args;
	if (!getcwd(path, sizeof(path)))
	{
		print_error("pwd: error retrieving current directory");
		return (1);
	}
	printf("%s\n", path);
	return (0);
}

int	ft_exit(t_minishell *shell, char **args)
{
	int	status;

	if (args[1])
		status = ft_atoi(args[1]);
	else
		status = shell->exit_status;
	printf("exit shell ...\n");
	exit(status);
}
