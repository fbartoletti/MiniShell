/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:57:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 10:39:56 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sort_env_array(char **env, int size)
{
	int		i;
	int		j;
	int		swapped;
	char	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		swapped = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

void	print_export_var(char *var)
{
	char	*equal_sign;
	int		len;

	ft_putstr_fd("declare -x ", 1);
	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
	{
		len = equal_sign - var + 1;
		write(1, var, len);
		ft_putchar_fd('"', 1);
		ft_putstr_fd(equal_sign + 1, 1);
		ft_putchar_fd('"', 1);
	}
	else
		ft_putstr_fd(var, 1);
	ft_putchar_fd('\n', 1);
}

int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	execute_builtin_command(t_terminal *term, t_command_info *cmd)
{
	if (!ft_strcmp(cmd->matrix[0], "echo"))
		g_last_status = cmd_echo(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "cd"))
		g_last_status = cmd_cd(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "pwd"))
		g_last_status = cmd_pwd(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "export"))
		g_last_status = cmd_export(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "unset"))
		g_last_status = cmd_unset(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "env"))
		g_last_status = cmd_env(term, cmd->matrix);
	else if (!ft_strcmp(cmd->matrix[0], "exit"))
		g_last_status = cmd_exit(term, cmd->matrix);
}

void	identify_builtin(t_command_info *cmd)
{
	if (!cmd || !cmd->matrix || !cmd->matrix[0])
		return ;
	cmd->builtin.is_builtin = is_builtin_cmd(cmd->matrix[0]);
	cmd->builtin.is_echo = (ft_strcmp(cmd->matrix[0], "echo") == 0);
	cmd->builtin.is_cd = (ft_strcmp(cmd->matrix[0], "cd") == 0);
	cmd->builtin.is_pwd = (ft_strcmp(cmd->matrix[0], "pwd") == 0);
	cmd->builtin.is_export = (ft_strcmp(cmd->matrix[0], "export") == 0);
	cmd->builtin.is_unset = (ft_strcmp(cmd->matrix[0], "unset") == 0);
	cmd->builtin.is_env = (ft_strcmp(cmd->matrix[0], "env") == 0);
	cmd->builtin.is_exit = (ft_strcmp(cmd->matrix[0], "exit") == 0);
}
