/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 11:38:15 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:22:45 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_command_path(t_minishell *shell, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;
	
	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(shell->env, "PATH");
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
			int j = 0;
			while (paths[j])
				free(paths[j++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
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
		dup2(fd, STDIN_FILENO);
	}
	else if (redir->type == TOKEN_HEREDOC)
	{
		fd = handle_heredoc(redir, shell);
		if (fd < 0)
			return (-1);
		dup2(fd, STDIN_FILENO);
	}
	else if (redir->type == TOKEN_REDIR_OUT)
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (print_error("Error opening output file"), -1);
		dup2(fd, STDOUT_FILENO);
	}
	else
	{
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return (print_error("Error opening output file"), -1);
		dup2(fd, STDOUT_FILENO);
	}
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
	char	*real_delimiter;
	int		quote_mode;

	if (pipe(pipe_fd) < 0)
		return (-1);
		
	content = init_heredoc(redir->file, shell, &real_delimiter, &quote_mode);
	if (!content)
		return (-1);
		
	content = handle_heredoc_loop(content, real_delimiter, quote_mode, shell);
	
	if (content)
	{
		write(pipe_fd[1], content, ft_strlen(content));
		free(content);
	}
	
	close(pipe_fd[1]);
	cleanup_heredoc(quote_mode, real_delimiter, shell);
	
	return (pipe_fd[0]);
}
