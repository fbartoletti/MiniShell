/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:14:23 by barto             #+#    #+#             */
/*   Updated: 2025/02/27 10:24:15 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_error(char *name, char *value)
{
	display_error("export: not a valid identifier");
	free(name);
	free(value);
	return (1);
}

int	handle_no_equal(t_terminal *term, char *name, char *value)
{
	if (find_env_var_index(term->new_env, name) == -1)
		add_new_env_var(term, ft_strdup_safe(name));
	free(name);
	free(value);
	return (0);
}

t_redirect_node	*create_heredoc_queue(void)
{
	return (NULL);
}

void	add_to_heredoc_queue(t_redirect_node **queue, char *delimiter)
{
	t_redirect_node	*new;
	t_redirect_node	*current;

	new = alloc_mem(sizeof(t_redirect_node));
	new->heredoc = alloc_mem(sizeof(t_heredoc_data));
	new->heredoc->delimiter = ft_strdup_safe(delimiter);
	new->heredoc->temp_filename = NULL;
	new->heredoc->expand = TRUE;
	new->heredoc->index = 0;
	new->next = NULL;
	if (!*queue)
	{
		*queue = new;
		return ;
	}
	current = *queue;
	while (current->next)
		current = current->next;
	current->next = new;
}

char	*process_heredoc_queue(t_terminal *term, t_redirect_node *queue)
{
	t_redirect_node	*current;
	char			*final_content;

	current = queue;
	final_content = NULL;
	while (current)
	{
		current->heredoc->temp_filename = generate_heredoc_filename(
				current->heredoc->index);
		if (!handle_heredoc_input(current, term))
		{
			return (NULL);
		}
		if (!final_content)
			final_content = ft_strdup_safe(current->fd_name);
		current = current->next;
	}
	return (final_content);
}
