/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:38:15 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 10:05:44 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_cmd_path(t_terminal *term, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;
	
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup_safe(cmd));
	path_env = get_env_from_array(term->new_env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	free(path_env);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		char *temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		
		if (access(full_path, X_OK) == 0)
		{
			free_string_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_string_array(paths);
	return (NULL);
}

int	setup_redirection(t_redirect_node *redir, t_terminal *term)
{
	int	fd;

	(void)term;
	if (redir->type.is_infile)
	{
		fd = open(redir->fd_name, O_RDONLY);
		if (fd < 0)
			return (display_error("No such file or directory"), -1);
		dup2(fd, STDIN_FILENO);
	}
	else if (redir->type.is_heredoc)
	{
		fd = handle_heredoc_input(redir);
		if (fd < 0)
			return (-1);
		dup2(fd, STDIN_FILENO);
	}
	else if (redir->type.is_outfile)
	{
		fd = open(redir->fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (display_error("Error opening output file"), -1);
		dup2(fd, STDOUT_FILENO);
	}
	else if (redir->type.is_append)
	{
		fd = open(redir->fd_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (display_error("Error opening output file"), -1);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (0);
}

int handle_heredoc_input(t_redirect_node *redir)
{
	int		pipe_fd[2];
	char	*content;
	char	*line;
	char	*temp;

	content = NULL;
	line = NULL;
	temp = NULL;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	content = ft_strdup("");
	if (!content)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break;
		}
		temp = content;
		content = ft_strjoin(content, line);
		free(temp);
		free(line);
		temp = content;
		content = ft_strjoin(content, "\n");
		free(temp);
	}
	if (content)
	{
		write(pipe_fd[1], content, ft_strlen(content));
		free(content);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

char	*get_env_from_array(char **env, const char *name)
{
	int	name_len;
	int	i;
	
	name_len = ft_strlen(name);
	i = 0;
	if (!name)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (ft_strdup(env[i] + name_len + 1));
		i++;
	}
	return (NULL);
}
