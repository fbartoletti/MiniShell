/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbartole <fbartole@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:13:44 by barto             #+#    #+#             */
/*   Updated: 2025/02/17 09:23:39 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void restore_io(t_terminal *term)
{
    if (dup2(term->stdin_copy, STDIN_FILENO) == -1)
        perror("dup2 stdin_copy");
    if (dup2(term->stdout_copy, STDOUT_FILENO) == -1)
        perror("dup2 stdout_copy");
    close(term->stdin_copy);
    close(term->stdout_copy);
}

void setup_input_redirects(t_redirect_node *redir)
{
    t_redirect_node *node;

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

void setup_output_redirects(t_redirect_node *redir)
{
    t_redirect_node *node;

    node = redir;
    while (node)
    {
        if (node->type.is_outfile)
            handle_output_redirect(node);
        if (node->type.is_append)
            handle_append_redirect(node);
        node = node->next;
    }
}
