/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barto <barto@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:29:56 by barto             #+#    #+#             */
/*   Updated: 2025/02/11 14:35:25 by barto            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_type	get_token_type(char *input, int i, int *len)
{
	if (input[i] == '<' && input[i + 1] == '<')
	{
		*len = 2;
		return (TOKEN_HEREDOC);
	}
	if (input[i] == '>' && input[i + 1] == '>')
	{
		*len = 2;
		return (TOKEN_REDIR_APPEND);
	}
	if (input[i] == '<')
		return (TOKEN_REDIR_IN);
	if (input[i] == '>')
		return (TOKEN_REDIR_OUT);
	return (TOKEN_PIPE);
}

int	create_special_token(t_token **tokens, char *value, t_token_type type)
{
	t_token	*new_token;

	if (!value)
		return (0);
	new_token = create_token(type, value);
	if (!new_token)
		return (0);
	add_token(tokens, new_token);
	return (1);
}

int	handle_heredoc_delimiter(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*value;
	int		quote_mode;

	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
	start = *i;
	quote_mode = (input[*i] == '\'');
	if (quote_mode)
		(*i)++;
	while (input[*i] && ((quote_mode && input[*i] != '\'') || 
		(!quote_mode && !is_whitespace(input[*i]))))
		(*i)++;
	if (quote_mode && input[*i] == '\'')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (0);
	add_token(tokens, create_token(TOKEN_WORD, value));
	return (1);
}