/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:23:39 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*read_heredoc_input(char *delimiter, t_minishell *shell)
{
	char	*content;
	char	*line;
	int		quote_mode;
	char	*real_delimiter;

	content = ft_strdup("");
	if (!content)
		return NULL;
	quote_mode = (delimiter[0] == '\'' && 
				 delimiter[ft_strlen(delimiter) - 1] == '\'');
	if (quote_mode)
		real_delimiter = ft_strtrim(delimiter, "'");
	else
		real_delimiter = ft_strdup(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, real_delimiter) == 0)
		{
			free(line);
			break;
		}
		if (!quote_mode && ft_strchr(line, '$'))
		{
			char *expanded = expand_variables(shell, line);
			free(line);
			line = expanded;
		}
		content = append_line_to_content(content, line);
		free(line);
		if (!content)
		{
			free(real_delimiter);
			return NULL;
		}
	}
	free(real_delimiter);
	return content;
}

int	handle_heredoc_token(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*value;

	(*i) += 2;
	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !is_whitespace(input[*i]))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	add_token(tokens, create_token(TOKEN_HEREDOC, value));
	
	return (1);
}

void	handle_command_error(char *cmd)
{
	struct stat	st;
	char		*error;

	if (stat(cmd, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			error = "is a directory";
		else if (!(st.st_mode & S_IXUSR))
			error = "permission denied";
		else
			error = "command not found";
	}
	else
		error = "no such file or directory";
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

void check_command_path(char *cmd_path, char *cmd)
{
	if (!cmd_path)
	{
		handle_command_error(cmd);
		exit(127);
	}
}

char	*append_line_to_content(char *content, char *line)
{
	char	*temp;
	char	*new_content;

	temp = content;
	new_content = ft_strjoin(content, line);
	free(temp);
	if (!new_content)
		return (NULL);
	temp = new_content;
	new_content = ft_strjoin(new_content, "\n");
	free(temp);
	return (new_content);
}
