/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:22:32 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 14:59:45 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipes(t_executor *exec)
{
	if (exec->prev_pipe != -1)
	{
		close(exec->prev_pipe);
		exec->prev_pipe = -1;
	}
	if (exec->pipe_fd[0] != -1)
	{
		close(exec->pipe_fd[0]);
		exec->pipe_fd[0] = -1;
	}
	if (exec->pipe_fd[1] != -1)
	{
		close(exec->pipe_fd[1]);
		exec->pipe_fd[1] = -1;
	}
}

void	init_executor(t_executor *exec)
{
	exec->pipe_fd[0] = -1;
	exec->pipe_fd[1] = -1;
	exec->prev_pipe = -1;
	exec->pid = -1;
	exec->status = 0;
}

void	wait_all_processes(t_minishell *shell, t_command *cmd)
{
	int		status;
	pid_t	pid;
	int		cmd_count;

	cmd_count = 0;
	while (cmd)
	{
		if (cmd->args && cmd->args[0])
			cmd_count++;
		cmd = cmd->next;
	}
	while (cmd_count > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break;
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		cmd_count--;
	}
}

int	setup_builtin_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout,
	t_minishell *shell)
{
	t_redir	*redir;

	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin < 0 || *saved_stdout < 0)
		return (-1);
	redir = cmd->redirs;
	while (redir)
	{
		if (setup_redirection(redir, shell) < 0)
		{
			restore_redirections(*saved_stdin, *saved_stdout);
			return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
