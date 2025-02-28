/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:22:32 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 11:44:15 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	close_pipe_fds(int *pipe_fd, int prev_pipe)
{
	if (prev_pipe != -1)
		close(prev_pipe);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}

void	init_pipe_fds(int *pipe_fd, int *prev_pipe)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	*prev_pipe = -1;
}

void	wait_for_processes(int cmd_count)
{
	int		status;
	pid_t	pid;

	while (cmd_count > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_last_status = 128 + WTERMSIG(status);
		cmd_count--;
	}
}

int	count_valid_commands(t_command_info *cmd)
{
	int	cmd_count;

	cmd_count = 0;
	while (cmd)
	{
		if (cmd->matrix && cmd->matrix[0])
			cmd_count++;
		cmd = cmd->next;
	}
	return (cmd_count);
}

void	save_io_descriptors(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin < 0 || *saved_stdout < 0)
	{
		ft_putstr_fd("minishell: dup error\n", 2);
		return ;
	}
}
