/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 12:33:10 by ubuntu            #+#    #+#             */
/*   Updated: 2025/02/17 09:20:46 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_hdoc	*create_hdoc_node(char *delimiter)
{
	t_hdoc	*new;

	new = safe_malloc(sizeof(t_hdoc));
	new->delimiter = ft_strdup(delimiter);
	new->next = NULL;
	return (new);
}

void	add_hdoc(t_hdoc **list, t_hdoc *new)
{
	t_hdoc	*temp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

char	*process_hdoc_content(t_minishell *shell, t_hdoc *hdocs)
{
	t_hdoc	*current;
	char	*content;
	char	*final_content;
	char	*line;
	char	*temp;

	current = hdocs;
	final_content = ft_strdup("");
	while (current)
	{
		content = ft_strdup("");
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				free(content);
				return (final_content);
			}
			if (!ft_strcmp(line, current->delimiter))
			{
				free(line);
				break;
			}
			if (!ft_strchr(current->delimiter, '\''))
				line = expand_variables(shell, line);
			temp = content;
			content = ft_strjoin(content, line);
			free(temp);
			temp = content;
			content = ft_strjoin(content, "\n");
			free(temp);
			free(line);
		}
		if (!current->next)
		{
			free(final_content);
			return (content);
		}
		free(final_content);
		final_content = content;
		current = current->next;
	}
	return (final_content);
}

void	free_hdoc_list(t_hdoc *list)
{
	t_hdoc	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->delimiter);
		free(temp);
	}
}
