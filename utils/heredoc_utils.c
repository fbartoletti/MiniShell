/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:41:13 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 11:08:15 by barto            ###   ########.fr       */
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

void	free_heredoc_data(int expand_mode, char *real_delimiter,
	t_terminal *term)
{
	if (!expand_mode)
		free(real_delimiter);
	(void)term;
}

char	*read_heredoc_lines(char *content, char *real_delimiter,
	int expand_mode, t_terminal *term)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, real_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand_mode && ft_strchr(line, '$'))
		{
			char *expanded = expand_vars(term, line);
			free(line);
			line = expanded;
		}
		content = append_to_content(content, line);
		free(line);
		if (!content)
			return (NULL);
	}
	return (content);
}
