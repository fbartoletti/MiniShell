/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:41:13 by barto             #+#    #+#             */
/*   Updated: 2025/02/16 12:34:05 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*init_heredoc(char *delimiter, t_minishell *shell,
	char **real_delimiter, int *quote_mode)
{
	char	*content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	shell->in_heredoc = 1;
	*quote_mode = 0;
	*real_delimiter = delimiter;
	if (delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
	{
		*quote_mode = 1;
		*real_delimiter = ft_strtrim(delimiter, "'");
	}
	return (content);
}

char	*process_heredoc_line(char *line, t_minishell *shell, int quote_mode)
{
	char	*expanded;

	if (!quote_mode && ft_strchr(line, '$'))
	{
		expanded = expand_variables(shell, line);
		free(line);
		return (expanded);
	}
	return (line);
}

void	cleanup_heredoc(int quote_mode, char *real_delimiter,
	t_minishell *shell)
{
	if (quote_mode)
		free(real_delimiter);
	shell->in_heredoc = 0;
}

char	*handle_heredoc_loop(char *content, char *real_delimiter,
	int quote_mode, t_minishell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, real_delimiter) == 0)
		{
			free(line);
			break;
		}
		line = process_heredoc_line(line, shell, quote_mode);
		content = append_line_to_content(content, line);
		free(line);
		if (!content)
		{
			cleanup_heredoc(quote_mode, real_delimiter, shell);
			return (NULL);
		}
	}
	return (content);
}
