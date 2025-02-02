/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/02/02 13:17:00 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_pipe_error(t_minishell *shell)
{
	ft_putendl_fd("minishell: pipe error", 2);
	shell->exit_status = 1;
}

int	prepare_pipes(t_executor *exec, t_command *current, t_minishell *shell)
{
	if (current->next)
	{
		if (pipe(exec->pipe_fd) < 0)
		{
			handle_pipe_error(shell);
			return (0);
		}
	}
	else
	{
		exec->pipe_fd[0] = -1;
		exec->pipe_fd[1] = -1;
	}
	return (1);
}

int	execute_commands(t_minishell *shell)
{
	t_command	*current;
	t_executor	exec;
	int			ret;

	current = shell->commands;
	init_executor(&exec);
	ret = 0;
	while (current && ret >= 0)
	{
		if (current->next)
		{
			if (pipe(exec.pipe_fd) < 0)
			{
				ft_putendl_fd("minishell: pipe error", 2);
				return (-1);
			}
		}
		else
		{
			exec.pipe_fd[0] = -1;
			exec.pipe_fd[1] = -1;
		}
		ret = execute_single_command(shell, current, &exec);
		if (exec.pipe_fd[1] != -1)
		{
			close(exec.pipe_fd[1]);
			exec.pipe_fd[1] = -1;
		}
		if (exec.prev_pipe != -1)
		{
			close(exec.prev_pipe);
			exec.prev_pipe = -1;
		}
		exec.prev_pipe = exec.pipe_fd[0];
		exec.pipe_fd[0] = -1;
		current = current->next;
	}
	if (exec.prev_pipe != -1)
		close(exec.prev_pipe);
	wait_all_processes(shell, shell->commands);
	return (ret);
}
