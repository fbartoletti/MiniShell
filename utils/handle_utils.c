/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:14:23 by barto             #+#    #+#             */
/*   Updated: 2025/02/10 11:14:40 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_error(char *name, char *value)
{
	print_error("export: not a valid identifier");
	free(name);
	free(value);
	return (1);
}

int	handle_no_equal(t_minishell *shell, char *name, char *value)
{
	if (find_env_index(shell->env, name) == -1)
		add_new_env_var(shell, ft_strdup(name));
	free(name);
	free(value);
	return (0);
}

t_heredoc_queue *create_heredoc_queue(void)
{
	return (NULL);
}

void	add_to_heredoc_queue(t_heredoc_queue **queue, char *delimiter)
{
	t_heredoc_queue	*new;
	t_heredoc_queue	*current;

	new = safe_malloc(sizeof(t_heredoc_queue));
	new->delimiter = ft_strdup(delimiter);
	new->content = NULL;
	new->next = NULL;
	if (!*queue)
	{
		*queue = new;
		return;
	}
	current = *queue;
	while (current->next)
		current = current->next;
	current->next = new;
}

char	*process_heredoc_queue(t_minishell *shell, t_heredoc_queue *queue)
{
	t_heredoc_queue	*current;
	char			*final_content;

	current = queue;
	final_content = NULL;
	while (current)
	{
		shell->in_heredoc = 1;
		current->content = read_heredoc_input(current->delimiter, shell);
		if (!current->content)
		{
			shell->in_heredoc = 0;
			return NULL;
		}
		if (!final_content)
			final_content = current->content;
		current = current->next;
	}
	shell->in_heredoc = 0;
	return final_content;
}
