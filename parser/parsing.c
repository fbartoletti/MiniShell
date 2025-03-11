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

void	add_command_to_shell(t_terminal *term, t_command_info *cmd)
{
	t_command_info	*tmp;

	identify_builtin(cmd);
	if (!term->commands)
	{
		term->commands = cmd;
		cmd->prev = cmd;
	}
	else
	{
		tmp = term->commands;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
		cmd->prev = tmp;
		term->commands->prev = cmd;
	}
}

static int	validate_redirections(t_argument *arg)
{
	if (!arg->next || !arg->next->str)
	{
		ft_putstr_fd(ERR_TOK, 2);
		return (0);
	}
	return (1);
}

static void	handle_word_token(t_command_info *cmd, char *word)
{
	char	**new_args;
	int		i;
	int		j;

	i = 0;
	while (cmd->matrix && cmd->matrix[i])
		i++;
	new_args = alloc_mem(sizeof(char *) * (i + 2));
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->matrix[j];
		j++;
	}
	new_args[i] = ft_strdup_safe(word);
	new_args[i + 1] = NULL;
	free(cmd->matrix);
	cmd->matrix = new_args;
}

int	handle_token(t_terminal *term, t_argument *arg, t_command_info *cmd)
{
	if (!arg || !cmd)
		return (0);
	if (arg->token.is_pipe)
	{
		if (!cmd->matrix && !cmd->redirects)
			return (display_error(ERR_PIPE), 0);
		identify_builtin(cmd);
		add_command_to_shell(term, cmd);
		return (1);
	}
	else if (is_redir_token(arg->token))
	{
		if (!validate_redirections(arg))
			return (0);
		process_redirections(cmd, arg);
		return (2);
	}
	else if (!arg->token.is_token)
	{
		if (!arg->prev || (arg->prev && !arg->prev->token.is_heredoc))
			handle_word_token(cmd, arg->str);
	}
	return (1);
}

int	process_input_line(t_terminal *term, char *line)
{
	t_argument		*current;
	t_command_info	*cmd;

	term->args = tokenize_input(line);
	if (!term->args)
		return (0);
	current = term->args;
	cmd = create_cmd();
	if (!cmd)
		return (0);
	if (processing(&current, term, &cmd) == 0)
		return (0);
	if (cmd->matrix || cmd->redirects)
	{
		identify_builtin(cmd);
		add_command_to_shell(term, cmd);
	}
	else
	{
		free_cmd_content(cmd);
		free(cmd);
	}
	return (1);
}
