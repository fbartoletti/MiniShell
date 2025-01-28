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

t_redir	*create_redirection(t_token_type type, char *file)
{
	t_redir	*redir;
	
	redir = safe_malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

void	add_redir_to_cmd(t_command *cmd, t_redir *redir)
{
	t_redir	*tmp;
	
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}
