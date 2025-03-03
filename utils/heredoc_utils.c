/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:41:13 by fbartole          #+#    #+#             */
/*   Updated: 2025/03/03 09:46:29 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*init_heredoc_data(char *delimiter, t_terminal *term, 
					   char **real_delimiter, int *expand_mode)
{
	char	*content;

	(void)term;
	content = ft_strdup_safe("");
	if (!content)
		return (NULL);
	*expand_mode = 1;
	*real_delimiter = delimiter;
	if (delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
	{
		*expand_mode = 0;
		*real_delimiter = ft_strtrim(delimiter, "'");
	}
	return (content);
}

char	*process_heredoc_content(char *line, t_terminal *term, int expand_mode)
{
	char	*expanded;

	if (expand_mode && ft_strchr(line, '$'))
	{
		expanded = expand_vars(term, line);
		free(line);
		return (expanded);
	}
	return (line);
}

void	free_heredoc_data(int expand_mode, char *real_delimiter, t_terminal *term)
{
	if (!expand_mode)
		free(real_delimiter);
	(void)term;
}

char	*read_heredoc_lines(char *content, char *real_delimiter, 
			int expand_mode, t_terminal *term)
{
	char	*line;
	char	*temp;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, real_delimiter) == 0)
		{
			free(line);
			break;
		}
		if (expand_mode && ft_strchr(line, '$'))
		{
			temp = expand_vars(term, line);
			free(line);
			line = temp;
		}
		temp = content;
		content = ft_strjoin(content, line);
		free(temp);
		temp = content;
		content = ft_strjoin(content, "\n");
		free(temp);
		free(line);
		if (!content)
			return (NULL);
	}
	return (content);
}

void	assign_heredoc_indices(t_terminal *term)
{
	t_command_info	*cmd;
	t_redirect_node	*redir;
	int				index;
	
	index = 0;
	cmd = term->commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type.is_heredoc)
			{
				redir->heredoc->index = index;
				index++;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
