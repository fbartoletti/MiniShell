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
	t_hdoc	*hdocs;
	char	*content;
	int		pipe_fd[2];
	t_token	*curr;
	t_hdoc 	*last_hdoc;

	hdocs = NULL;
	last_hdoc = NULL;
	curr = shell->tokens;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC && curr->next)
		{
			t_hdoc *new_hdoc = create_hdoc_node(curr->next->value);
			if (!hdocs)
				hdocs = new_hdoc;
			else
				last_hdoc->next = new_hdoc;
			last_hdoc = new_hdoc;
		}
		curr = curr->next;
	}
	if (!hdocs)
		hdocs = create_hdoc_node(redir->file);
	if (pipe(pipe_fd) < 0)
	{
		free_hdoc_list(hdocs);
		return (-1);
	}
	shell->in_heredoc = 1;
	content = process_hdoc_content(shell, hdocs);
	shell->in_heredoc = 0;
	if (content)
	{
		write(pipe_fd[1], content, ft_strlen(content));
		free(content);
	}
	close(pipe_fd[1]);
	free_hdoc_list(hdocs);
	return (pipe_fd[0]);
}
