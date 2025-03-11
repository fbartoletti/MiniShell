/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:24:58 by barto             #+#    #+#             */
/*   Updated: 2025/03/03 13:36:16 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_variable(t_terminal *term, char *str, int *i, char *result)
{
	char	*expanded;
	char	*new_result;

	(*i)++;
	if (!str[*i] || str[*i] == ' ')
		return (add_char(result, '$'));
	if (str[*i] == '?')
	{
		expanded = get_status_str(term);
		(*i)++;
	}
	else
		expanded = get_env_var_value(term->new_env, extract_var_name(str, i));
	if (!expanded)
		return (result);
	new_result = concat_strings(result, expanded);
	free(result);
	free(expanded);
	return (new_result);
}

char	*add_char(char *str, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = concat_strings(str, temp);
	free(str);
	return (new_result);
}

int	expand_redirect_paths(t_terminal *term, t_command_info *cmd)
{
	t_redirect_node	*redir;
	char			*expanded;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->fd_name && !redir->type.is_heredoc)
		{
			expanded = expand_vars(term, redir->fd_name);
			if (expanded)
			{
				free(redir->fd_name);
				redir->fd_name = expanded;
			}
		}
		redir = redir->next;
	}
	return (0);
}

void	process_expansions(t_terminal *term)
{
	t_command_info	*cmd;

	cmd = term->commands;
	while (cmd)
	{
		expand_cmd_args(term, cmd);
		expand_redirect_paths(term, cmd);
		cmd = cmd->next;
	}
}
