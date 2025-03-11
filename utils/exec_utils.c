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
	full_path = create_path(&paths, cmd);
	if (full_path)
		return (full_path);
	free_string_array(paths);
	return (NULL);
}

int	handle_heredoc_input(t_redirect_node *redir)
{
	int		pipe_fd[2];
	char	*content;

	content = NULL;
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
	heredoc_rl(&content, redir);
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
