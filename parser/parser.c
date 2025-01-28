/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:05:53 by barto             #+#    #+#             */
/*   Updated: 2025/01/09 10:27:26 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_command_content(t_command *cmd)
{
	t_redir	*tmp_redir;
	t_redir	*next_redir;
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	tmp_redir = cmd->redirs;
	while (tmp_redir)
	{
		next_redir = tmp_redir->next;
		free(tmp_redir->file);
		free(tmp_redir);
		tmp_redir = next_redir;
	}
}

t_command	*create_command(void)
{
	t_command	*cmd;
	
	cmd = safe_malloc(sizeof(t_command));
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **commands, t_command *new)
{
	t_command	*tmp;

	if (!*commands)
	{
		*commands = new;
		return;
	}
	tmp = *commands;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
			type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

void	handle_redirection(t_command *cmd, t_token *token)
{
	t_redir	*redir;
	
	if (!token->next || token->next->type != TOKEN_WORD)
	{
		print_error(ERR_REDIR);
		return;
	}
	redir = create_redirection(token->type, token->next->value);
	if (!redir)
		return;
	add_redir_to_cmd(cmd, redir);
}
