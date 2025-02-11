/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/11 14:42:27 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*read_heredoc_input(char *delimiter, t_minishell *shell)
{
	char	*content;
	char	*real_delimiter;
	int		quote_mode;

	content = init_heredoc(delimiter, shell, &real_delimiter, &quote_mode);
	if (!content)
		return (NULL);
	content = handle_heredoc_loop(content, real_delimiter, quote_mode, shell);
	cleanup_heredoc(quote_mode, real_delimiter, shell);
	return (content);
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
