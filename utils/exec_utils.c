/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:38:15 by barto             #+#    #+#             */
/*   Updated: 2025/01/07 12:50:12 by barto            ###   ########.fr       */
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

int	setup_redirection(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (handle_heredoc(redir));
	if (fd < 0)
		return (-1);
	
	if (redir->type == TOKEN_REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
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
}
