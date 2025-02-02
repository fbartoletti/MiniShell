/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:36:43 by barto             #+#    #+#             */
/*   Updated: 2025/02/02 13:15:05 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_single_command(t_minishell *shell, t_command *cmd, t_executor *exec)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]) && !cmd->next)
		return (execute_builtin(shell, cmd));
	exec->pid = fork();
	if (exec->pid < 0)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (exec->pid == 0)
	{
		handle_child_signals();
		if (exec->prev_pipe != -1)
		{
			dup2(exec->prev_pipe, STDIN_FILENO);
			close(exec->prev_pipe);
		}
		if (exec->pipe_fd[1] != -1)
		{
			dup2(exec->pipe_fd[1], STDOUT_FILENO);
			close(exec->pipe_fd[1]);
		}
		if (exec->pipe_fd[0] != -1)
			close(exec->pipe_fd[0]);
		handle_redirections(cmd);
		if (is_builtin(cmd->args[0]))
			exit(execute_builtin(shell, cmd));
		execute_external(shell, cmd);
	}
	handle_parent_signals();
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
