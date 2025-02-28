/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 12:33:10 by ubuntu            #+#    #+#             */
/*   Updated: 2025/02/27 11:20:17 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redirect_node	*create_heredoc_node(char *delimiter)
{
	t_redirect_node	*new;

	new = alloc_mem(sizeof(t_redirect_node));
	new->heredoc = alloc_mem(sizeof(t_heredoc_data));
	new->heredoc->delimiter = ft_strdup_safe(delimiter);
	new->heredoc->temp_filename = NULL;
	new->heredoc->expand = TRUE;
	new->heredoc->index = 0;
	new->fd_name = NULL;
	new->type.is_heredoc = TRUE;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_heredoc_node(t_redirect_node **list, t_redirect_node *new)
{
	t_redirect_node	*temp;

	if (!*list)
	{
		*list = new;
		new->prev = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	(*list)->prev = new;
}

char	*process_heredoc_contents(t_terminal *term, t_redirect_node *redirects)
{
	t_redirect_node	*current;
	char			*content;
	char			*final_content;
	char			*line;
	char			*temp;

	current = redirects;
	final_content = ft_strdup_safe("");
	while (current)
	{
		if (current->type.is_heredoc)
		{
			content = ft_strdup_safe("");
			while (1)
			{
				line = readline("> ");
				if (!line)
				{
					free(content);
					return (final_content);
				}
				if (!ft_strcmp(line, current->heredoc->delimiter))
				{
					free(line);
					break ;
				}
				if (current->heredoc->expand)
					line = expand_vars(term, line);
				temp = content;
				content = ft_strjoin(content, line);
				free(temp);
				temp = content;
				content = ft_strjoin(content, "\n");
				free(temp);
				free(line);
			}
			if (!(current->next && current->next->type.is_heredoc))
			{
				free(final_content);
				return (content);
			}
			free(final_content);
			final_content = content;
		}
		current = current->next;
	}
	return (final_content);
}

void	free_heredoc_list(t_redirect_node *list)
{
	t_redirect_node	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->heredoc)
		{
			if (temp->heredoc->delimiter)
				free(temp->heredoc->delimiter);
			if (temp->heredoc->temp_filename)
				free(temp->heredoc->temp_filename);
			free(temp->heredoc);
		}
		if (temp->fd_name)
			free(temp->fd_name);
		free(temp);
	}
}
