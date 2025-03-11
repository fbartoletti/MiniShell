/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor-utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 02:56:03 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 02:56:08 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_dup(t_terminal *term)
{
	term->stdin_copy = dup(STDIN_FILENO);
	term->stdout_copy = dup(STDOUT_FILENO);
	if (term->stdin_copy == -1 || term->stdout_copy == -1)
	{
		perror("dup");
		return ;
	}
}

int	count_heredocs(t_redirect_node *redir)
{
	int	count;

	count = 0;
	while (redir)
	{
		if (redir->type.is_heredoc)
			count++;
		redir = redir->next;
	}
	return (count);
}

void	populate_heredocs(t_redirect_node *redir,
t_command_info **cmd, t_redirect_node ***heredocs)
{
	int	i;

	i = 0;
	redir = (*cmd)->redirects;
	while (redir)
	{
		if (redir->type.is_heredoc)
		{
			(*heredocs)[i] = redir;
			i++;
		}
		redir = redir->next;
	}
}

void	heredoc_process(t_redirect_node ***heredocs, int i, char **content)
{
	char	*line;
	char	*temp;

	while (1)
	{
		line = readline("");
		if (!line || ft_strcmp(line, (*heredocs)[i]->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		temp = (*content);
		(*content) = ft_strjoin((*content), line);
		free(temp);
		free(line);
		temp = (*content);
		(*content) = ft_strjoin((*content), "\n");
		free(temp);
		ft_putstr_fd("> ", 2);
	}
}

int	handle_heredocs(int count, t_redirect_node ***heredocs)
{
	int		i;
	int		pipe_fd[2];
	char	*content;

	i = 0;
	while (i < count)
	{
		if (pipe(pipe_fd) < 0)
		{
			ft_putstr_fd("minishell: errore pipe heredoc\n", 2);
			return (0);
		}
		content = ft_strdup_safe("");
		ft_putstr_fd("> ", 2);
		heredoc_process(heredocs, i, &content);
		write(pipe_fd[1], content, ft_strlen(content));
		free(content);
		close(pipe_fd[1]);
		(*heredocs)[i]->heredoc_fd = pipe_fd[0];
		i++;
	}
	return (1);
}
