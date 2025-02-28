/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:52:52 by barto             #+#    #+#             */
/*   Updated: 2025/01/04 16:54:57 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


t_redirect_node *create_redirect(t_redirect type, char *file)
{
	t_redirect_node *redir;
	
	redir = alloc_mem(sizeof(t_redirect_node));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd_name = ft_strdup_safe(file);
	if (!redir->fd_name)
	{
		free(redir);
		return (NULL);
	}
	redir->heredoc = NULL;
	redir->next = NULL;
	redir->prev = NULL;
	
	// Se è un heredoc, inizializza la struttura heredoc
	if (type.is_heredoc)
	{
		redir->heredoc = alloc_mem(sizeof(t_heredoc_data));
		if (!redir->heredoc)
		{
			free(redir->fd_name);
			free(redir);
			return (NULL);
		}
		redir->heredoc->delimiter = ft_strdup_safe(file);
		redir->heredoc->temp_filename = NULL;
		if (redir->fd_name[0] == '\'')
			redir->heredoc->expand = FALSE;
		else
			redir->heredoc->expand = TRUE;
		redir->heredoc->index = 0;
	}
	
	return (redir);
}

void add_redirect(t_command_info *cmd, t_redirect_node *redir)
{
	t_redirect_node *tmp;
	
	if (!cmd->redirects)
	{
		cmd->redirects = redir;
		redir->prev = redir;  // Lista circolare
	}
	else
	{
		tmp = cmd->redirects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
		redir->prev = tmp;
		cmd->redirects->prev = redir;  // Aggiorna il prev del primo nodo
	}
}

// Funzione per generare un nome di file temporaneo per heredoc
char *generate_heredoc_filename(int index)
{
	char *index_str;
	char *temp;
	char *filename;
	
	index_str = ft_itoa(index);
	temp = ft_strjoin(".here_doc_", index_str);
	free(index_str);
	
	if (!temp)
		return (NULL);
		
	filename = ft_strjoin(temp, ".tmp");
	free(temp);
	
	return (filename);
}

// Funzione per preparare tutti gli heredoc nei comandi
int prepare_heredocs(t_terminal *term)
{
	t_command_info *cmd;
	t_redirect_node *redir;
	static int heredoc_index = 0;
	
	cmd = term->commands;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type.is_heredoc)
			{
				heredoc_index++;
				redir->heredoc->index = heredoc_index;
				redir->heredoc->temp_filename = generate_heredoc_filename(heredoc_index);
				
				if (!redir->heredoc->temp_filename)
					return (0);
					
				// Qui potrebbe essere chiamata una funzione per leggere l'input dell'heredoc
				// handle_heredoc_input(redir, term);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	
	return (1);
}