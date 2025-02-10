/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 14:43:38 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*read_heredoc_input(char *delimiter, t_minishell *shell)
{
	char	*line;
	char	*content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	shell->in_heredoc = 1;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		content = append_line_to_content(content, line);
		free(line);
		if (!content)
			return (NULL);
	}
	shell->in_heredoc = 0;
	return (content);
}

int	handle_heredoc(t_redir *redir, t_minishell *shell)
{
	char	*content;
	int		pipe_fd[2];

	if (pipe(pipe_fd) < 0)
		return (-1);
	content = read_heredoc_input(redir->file, shell);
	if (!content)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	write(pipe_fd[1], content, ft_strlen(content));
	free(content);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
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
