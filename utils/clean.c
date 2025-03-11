/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:46:14 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 10:41:59 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_redirect_node(t_redirect_node *redir)
{
	t_redirect_node	*tmp;

	if (!redir)
		return ;
	if (redir->prev)
		redir->prev->next = NULL;
	while (redir)
	{
		tmp = redir->next;
		free(redir->fd_name);
		if (redir->heredoc)
		{
			free(redir->heredoc->delimiter);
			if (redir->heredoc->temp_filename)
			{
				if (access(redir->heredoc->temp_filename, F_OK) == 0)
					unlink(redir->heredoc->temp_filename);
				free(redir->heredoc->temp_filename);
			}
			free(redir->heredoc);
		}
		free(redir);
		redir = tmp;
	}
}

void	free_cmd_content(t_command_info *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->matrix)
	{
		i = 0;
		while (cmd->matrix[i])
		{
			free(cmd->matrix[i]);
			cmd->matrix[i] = NULL;
			i++;
		}
		free(cmd->matrix);
		cmd->matrix = NULL;
	}
	if (cmd->redirects)
	{
		free_redirect_node(cmd->redirects);
		cmd->redirects = NULL;
	}
}

void	free_env_list(t_environment **env)
{
	t_environment	*current;
	t_environment	*next;

	if (!*env)
		return ;
	if ((*env)->prev == *env)
		(*env)->prev = NULL;
	current = *env;
	while (current)
	{
		next = current->next;
		if (current->var)
			free(current->var);
		if (current->name)
			free(current->name);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

void	cleanup_child(t_terminal *term)
{
	term->args = NULL;
	term->commands = NULL;
	if (term->stdin_copy != -1)
	{
		close(term->stdin_copy);
		term->stdin_copy = -1;
	}
	if (term->stdout_copy != -1)
	{
		close(term->stdout_copy);
		term->stdout_copy = -1;
	}
}

void	cleanup_resources(void)
{
	int	fd;
	int	pid_status;

	pid_status = 1;
	while (pid_status > 0)
		pid_status = waitpid(-1, NULL, WNOHANG);
	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}
