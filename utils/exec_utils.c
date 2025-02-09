/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:38:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 16:12:19 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*join_path(char *path, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

char	*find_command_path(t_minishell *shell, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(shell->env, "PATH");
	paths = ft_split(path_env, ':');
	free(path_env);
	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	setup_redirection(t_redir *redir, t_minishell *shell)
{
	int	fd;

	if (redir->type == TOKEN_REDIR_IN)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd < 0)
			return (print_error("No such file or directory"), -1);
	}
	else if (redir->type == TOKEN_HEREDOC)
	{
		fd = handle_heredoc(redir, shell);
		if (fd < 0)
			return (-1);
		dup2(fd, STDIN_FILENO);
	}
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (print_error("No such file or directory"), -1);
	if (redir->type != TOKEN_REDIR_IN && redir->type != TOKEN_HEREDOC)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
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
