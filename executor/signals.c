/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:36:43 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:48:54 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_single_command(t_minishell *shell, t_command *cmd, t_executor *exec)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	expand_command_args(shell, cmd);
	if (is_builtin(cmd->args[0]) && !cmd->next)
		return (execute_builtin(shell, cmd));
	handle_parent_signals();
	exec->pid = fork();
	if (exec->pid < 0)
	{
		restore_signals();
		return (-1);
	}
	if (exec->pid == 0)
		execute_child_process(shell, cmd, exec);
	restore_signals();
	return (0);
}

void	handle_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	g_signal_received = 0;
}

void	handle_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}
