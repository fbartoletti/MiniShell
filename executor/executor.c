/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:31:12 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 14:22:15 by ubuntu           ###   ########.fr       */
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
	if (!term->commands || term->error)
		return ;
	setup_dup(term);
	if (term->commands && !term->commands->next
		&& term->commands->matrix && term->commands->matrix[0]
		&& is_builtin_cmd(term->commands->matrix[0]))
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

int	collect_heredocs_input(t_command_info *cmd)
{
	t_redirect_node	*redir;
	t_redirect_node	**heredocs;
	int				count;

	heredocs = NULL;
	redir = cmd->redirects;
	count = count_heredocs(redir);
	if (count == 0)
		return (1);
	heredocs = (t_redirect_node **)malloc(sizeof(t_redirect_node *) * count);
	if (!heredocs)
		return (0);
	populate_heredocs(redir, &cmd, &heredocs);
	if (handle_heredocs(count, &heredocs) == 0)
		return (0);
	free(heredocs);
	return (1);
}

void	execute_pipeline(t_terminal *term)
{
	int	pipe_fd[2];
	int	status;

	if (execute_commands(term, pipe_fd) != 0)
		return ;
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
