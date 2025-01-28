/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/01/07 13:14:05 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*read_heredoc_input(char *delimiter)
{
	char	*line;
	char	*content;
	char	*temp;

	content = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		temp = content;
		content = ft_strjoin(content, line);
		free(temp);
		temp = content;
		content = ft_strjoin(content, "\n");
		free(temp);
		free(line);
	}
	return (content);
}

int	handle_heredoc(t_redir *redir)
{
	char	*content;
	int		pipe_fd[2];
	
	if (pipe(pipe_fd) < 0)
		return (-1);
	content = read_heredoc_input(redir->file);
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
