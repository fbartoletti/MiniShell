/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 10:24:42 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_single_builtin(t_terminal *term)
{
	t_command_info	*cmd;

	cmd = term->commands;
	setup_input_redirects(cmd->redirects);
	setup_output_redirects(cmd->redirects);
	execute_builtin_command(term, cmd);
}

void	run_commands(t_terminal *term)
{ 
	if (!term->commands)
		return;
	term->stdin_copy = dup(STDIN_FILENO);
	term->stdout_copy = dup(STDOUT_FILENO);
	if (term->stdin_copy == -1 || term->stdout_copy == -1)
	{
		perror("dup");
		return;
	}
	if (term->commands && !term->commands->next && 
		term->commands->matrix && term->commands->matrix[0] && 
		is_builtin_cmd(term->commands->matrix[0]))
	{
		setup_input_redirects(term->commands->redirects);
		setup_output_redirects(term->commands->redirects);
		execute_builtin_command(term, term->commands);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		execute_pipeline(term);
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	restore_io(term);
}

int	process_heredocs_in_order(t_command_info *cmd)
{
	t_redirect_node	*redir;
	t_redirect_node	**heredocs;
	int	count;
	int	i;
	int	result;

	count = 0;
	result = 1;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type.is_heredoc)
			count++;
		redir = redir->next;
	}
	if (count == 0)
		return (1);
	heredocs = (t_redirect_node **)alloc_mem(sizeof(t_redirect_node *) * count);
	if (!heredocs)
		return (0);
	i = 0;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type.is_heredoc)
		{
			heredocs[i] = redir;
			i++;
		}
		redir = redir->next;
	}
	i = 0;
	while (i < count)
	{
		int fd = handle_heredoc_input(heredocs[i]);
		if (fd < 0)
		{
			result = 0;
			break;
		}
		heredocs[i]->heredoc_fd = fd;
		i++;
	}
	free(heredocs);
	return (result);
}

void	execute_pipeline(t_terminal *term)
{
	int				pipe_fd[2];
	int				prev_fd;
	t_command_info	*cmd;
	pid_t			pid;
	int				status;

	cmd = term->commands;
	while (cmd)
	{
		if (!process_heredocs_in_order(cmd))
		{
			ft_putstr_fd("minishell: errore heredoc\n", 2);
			return;
		}
		if (cmd->next && pipe(pipe_fd) < 0)
		{
			perror("pipe");
			if (prev_fd != -1)
				close(prev_fd);
			return;
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			return;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1)
			{
			if (cmd->next)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
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
				{
					run_external_command(term, cmd);
				}
			}
			exit(0);
			}
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			cmd = cmd->next;
		}
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			g_last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_last_status = 128 + WTERMSIG(status);
	}
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (1);
}
