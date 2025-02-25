/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:49:38 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 14:58:49 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	expand_command_args(t_minishell *shell, t_command *cmd)
{
	int		i;
	char	*expanded;

	i = 0;
	while (cmd->args[i])
	{
		expanded = expand_variables(shell, cmd->args[i]);
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
	return (0);
}

void	execute_child_process(t_minishell *shell, t_command *cmd, t_executor *exec)
{
	handle_child_signals();
	setup_pipes(exec);
	if (exec->pipe_fd[0] != -1)
		close(exec->pipe_fd[0]);
	handle_redirections(cmd, shell);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(shell, cmd));
	execute_external(shell, cmd);
	exit(127);
}

char	*create_quoted_value(char *value, char quote)
{
	char	*quoted_value;

	if (quote == '\'')
	{
		quoted_value = ft_strjoin("\1", value);
		free(value);
		if (!quoted_value)
			return (NULL);
		return (quoted_value);
	}
	return (value);
}
