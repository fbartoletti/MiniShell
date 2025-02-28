/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/28 14:20:54 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	restore_io(t_terminal *term)
{
	if (term->stdin_copy != -1)
	{
		dup2(term->stdin_copy, STDIN_FILENO);
		close(term->stdin_copy);
		term->stdin_copy = -1;
	}
	if (term->stdout_copy != -1)
	{
		dup2(term->stdout_copy, STDOUT_FILENO);
		close(term->stdout_copy);
		term->stdout_copy = -1;
	}
}

void	setup_input_redirects(t_redirect_node *redir)
{
	t_redirect_node	*node;

	if (!redir)
		return;
		
	node = redir;
	while (node)
	{
		if (node->type.is_heredoc)
			handle_heredoc_redirect(node);
		else if (node->type.is_infile)
			handle_input_redirect(node);
		node = node->next;
	}
}

void	setup_output_redirects(t_redirect_node *redir)
{
	t_redirect_node	*node;

	if (!redir)
		return;	
	node = redir;
	while (node)
	{
		if (node->type.is_outfile)
			handle_output_redirect(node);
		else if (node->type.is_append)
			handle_append_redirect(node);
		node = node->next;
	}
}
