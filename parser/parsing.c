/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:08:10 by barto             #+#    #+#             */
/*   Updated: 2025/02/25 15:04:16 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_command_to_shell(t_minishell *shell, t_command *cmd)
{
	t_command	*tmp;

	if (!shell->commands)
		shell->commands = cmd;
	else
	{
		tmp = shell->commands;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
	}
}

static int	validate_redirections(t_token *token)
{
	if (!token->next || token->next->type != TOKEN_WORD)
		return (print_error(ERR_REDIR), 0);
	return (1);
}

static void	handle_word_token(t_command *cmd, char *word)
{
	char	**new_args;
	int		i;
	int		j;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = safe_malloc(sizeof(char *) * (i + 2));
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = ft_strdup(word);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

int	parse_token(t_minishell *shell, t_token *token, t_command *cmd)
{
	if (token->type == TOKEN_PIPE)
	{
		if (!cmd->args && !cmd->redirs)
			return (print_error(ERR_PIPE), 0);
			
		add_command_to_shell(shell, cmd);
		return (1);
	}
	else if (is_redir_token(token->type))
	{
		if (!validate_redirections(token))
			return (0);
		handle_redirection(cmd, token);
		return (2);
	}
	else if (token->type == TOKEN_WORD && 
			 (!token->prev || token->prev->type != TOKEN_HEREDOC))
	{
		handle_word_token(cmd, token->value);
	}
	return (1);
}

int	parse_command(t_minishell *shell)
{
	t_token	*current;
	t_command	*cmd;
	int	ret;

	current = shell->tokens;
	cmd = create_command();
	while (current)
	{
		ret = parse_token(shell, current, cmd);
		if (ret == 0)
			return (0);
		if (ret == 2)
			current = current->next;
		else
			current = current->next;
	}
	if (cmd->args || cmd->redirs)
		add_command_to_shell(shell, cmd);
	else
		free_command(cmd);
	return (1);
}
