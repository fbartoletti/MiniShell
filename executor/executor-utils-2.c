/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor-utils-2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:54:54 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 03:54:56 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_prev_fd(int *prev_fd)
{
	if ((*prev_fd) != -1)
	{
		if (dup2((*prev_fd), STDIN_FILENO) == -1)
			perror("dup2 input");
		close((*prev_fd));
	}
}

void	parent(int *prev_fd, t_command_info *cmd,
int *pipe_fd, t_terminal *term)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	check_prev_fd(prev_fd);
	if (cmd->next)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror("dup2 output");
		close(pipe_fd[1]);
	}
	setup_input_redirects(cmd->redirects);
	setup_output_redirects(cmd->redirects);
	if (cmd->matrix && cmd->matrix[0])
	{
		if (is_builtin_cmd(cmd->matrix[0]))
		{
			execute_builtin_command(term, cmd);
			exit(g_last_status);
		}
		else
			run_external_command(term, cmd);
	}
	exit(0);
}

void	children(int *prev_fd, t_command_info *cmd, int *pipe_fd)
{
	if ((*prev_fd) != -1)
		close((*prev_fd));
	if (cmd->next)
	{
		close(pipe_fd[1]);
		(*prev_fd) = pipe_fd[0];
	}
	cmd = cmd->next;
}

int	generate_pid(int *prev_fd, t_command_info *cmd,
int *pipe_fd, t_terminal *term)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		if ((*prev_fd) != -1)
			close((*prev_fd));
		if (cmd->next)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		return (1);
	}
	if (pid == 0)
		parent(prev_fd, cmd, pipe_fd, term);
	else
		children(prev_fd, cmd, pipe_fd);
	return (0);
}

int	execute_commands(t_terminal *term)
{
	int				prev_fd;
	int				pipe_fd[2];
	t_command_info	*cmd;

	cmd = term->commands;
	while (cmd)
	{
		if (!collect_heredocs_input(cmd))
		{
			ft_putstr_fd("minishell: errore raccolta heredoc\n", 2);
			return (1);
		}
		if (cmd->next && pipe(pipe_fd) < 0)
		{
			perror("pipe");
			if (prev_fd != -1)
				close(prev_fd);
			return (1);
		}
		if (generate_pid(&prev_fd, cmd, pipe_fd, term) != 0)
			return (1);
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (0);
}
