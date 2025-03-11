/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_norm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgiampa <fgiampa@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:28:52 by fgiampa           #+#    #+#             */
/*   Updated: 2025/03/11 17:28:54 by fgiampa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*create_path(char ***paths, char *cmd)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	while ((*paths)[i])
	{
		temp = ft_strjoin((*paths)[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_string_array((*paths));
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

void	heredoc_rl(char **content, t_redirect_node *redir)
{
	char	*line;
	char	*temp;

	temp = NULL;
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->heredoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		temp = (*content);
		(*content) = ft_strjoin((*content), line);
		free(temp);
		free(line);
		temp = (*content);
		(*content) = ft_strjoin((*content), "\n");
		free(temp);
	}
}
