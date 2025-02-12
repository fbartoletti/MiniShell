/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:57:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/12 09:35:45 by barto            ###   ########.fr       */
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
			break;
		i++;
	}
}
void	print_export_var(char *var)
{
	char	*equal_sign;
	int	len;

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

int	execute_builtin_command(t_minishell *shell, t_command *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (ft_cd(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (ft_pwd(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "export"))
		return (ft_export(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (ft_unset(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (ft_env(shell, cmd->args));
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (ft_exit(shell, cmd->args));
	return (1);
}
