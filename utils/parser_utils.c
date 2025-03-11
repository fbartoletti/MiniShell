/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:52:52 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 10:10:14 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	setup_heredoc(t_redirect type, t_redirect_node **redir, char *file)
{
	if (type.is_heredoc)
	{
		(*redir)->heredoc = alloc_mem(sizeof(t_heredoc_data));
		if (!(*redir)->heredoc)
		{
			free((*redir)->fd_name);
			free((*redir));
			return (1);
		}
		(*redir)->heredoc->delimiter = ft_strdup_safe(file);
		(*redir)->heredoc->temp_filename = NULL;
		if ((*redir)->fd_name[0] == '\'')
			(*redir)->heredoc->expand = FALSE;
		else
			(*redir)->heredoc->expand = TRUE;
		(*redir)->heredoc->index = 0;
	}
	return (0);
}

t_redirect_node	*create_redirect(t_redirect type, char *file)
{
	t_redirect_node	*redir;

	redir = alloc_mem(sizeof(t_redirect_node));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd_name = ft_strdup_safe(file);
	redir->heredoc_fd = -1;
	if (!redir->fd_name)
	{
		free(redir);
		return (NULL);
	}
	redir->heredoc = NULL;
	redir->next = NULL;
	redir->prev = NULL;
	if (setup_heredoc(type, &redir, file) != 0)
		return (NULL);
	return (redir);
}

void	add_redirect(t_command_info *cmd, t_redirect_node *redir)
{
	t_redirect_node	*tmp;

	if (!cmd->redirects)
	{
		cmd->redirects = redir;
		redir->prev = redir;
	}
	else
	{
		tmp = cmd->redirects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
		redir->prev = tmp;
		cmd->redirects->prev = redir;
	}
}

char	*generate_heredoc_filename(int index)
{
	char	*index_str;
	char	*temp;
	char	*filename;

	index_str = ft_itoa(index);
	if (!index_str)
		return (NULL);
	temp = ft_strjoin(".here_doc_", index_str);
	free(index_str);
	if (!temp)
		return (NULL);
	filename = ft_strjoin(temp, ".tmp");
	free(temp);
	return (filename);
}
