/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:22:32 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 15:36:35 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipes(t_executor *exec)
{
	if (exec->prev_pipe != -1)
		close(exec->prev_pipe);
	if (exec->pipe_fd[0] != -1)
		close(exec->pipe_fd[0]);
	if (exec->pipe_fd[1] != -1)
		close(exec->pipe_fd[1]);
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
	int	status;
	int	last_pid;

	last_pid = -1;
	while (cmd)
	{
		if (cmd->args && cmd->args[0])
			last_pid = wait(&status);
		cmd = cmd->next;
	}
	if (last_pid != -1)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
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